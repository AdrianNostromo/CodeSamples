#include "ProfileStorageBase.h"
#include <sstream>
#include <base/app/config/AppConfig.h>
#include <iostream>
#include <string>
#include <cstring>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/exceptions/FileNotFoundException.h>
#include <base/exceptions/ResourceNotFoundException.h>
#include <base/util/StringUtil.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <base/app/IApp.h>
#include <base/threadsManager/IThreadsManager.h>
#include <base/threadsManager/managedThread/IManagedThread.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace base;

const int ProfileStorageBase::DataPathParts::FILE_SYSTEM_TARGET = Util_FileSystem::FSTarget::Private;

const std::string ProfileStorageBase::DataPathParts::FOLDER = "profile_data/";

const std::string ProfileStorageBase::DataPathParts::FILE_MAIN = "data";
const std::string ProfileStorageBase::DataPathParts::FILE_TEMP = "data.tmp";
const std::string ProfileStorageBase::DataPathParts::FILE_BACKUP = "data.backup_previous";

ProfileStorageBase::ProfileStorageBase()
	: super()
{
	mtxDataMap = newt std::mutex();
}

void ProfileStorageBase::init(IApp* app) {
	this->app = app;
}

void ProfileStorageBase::createMain() {
	super::createMain();

	managedThread = app->getThreadsManager()->newThread("ProfileStorage", std::bind(&ProfileStorageBase::run, this, std::placeholders::_1));
}

unsigned int ProfileStorageBase::run(base::IManagedThread* managedThread) {
	if (flush()) {
		// Something was saved, do a fast re-check.

		return 0;//sleepDurationMs
	}

	return 10;//sleepDurationMs
}

bool ProfileStorageBase::flush() {
	bool ret = false;

	while(isSaveRequested) {
		isSaveRequested = false;

		saveDataMap();
		ret = true;
	}

	return ret;
}

void ProfileStorageBase::saveDataMap() {
	std::unordered_map<std::string, LocalDataManagedValue*>* m = dataMap.getRawMap();
	int count = 0;
	std::stringstream dataMapSS{};
	mtxDataMap->lock();
	for(std::unordered_map<std::string, LocalDataManagedValue*>::iterator it = m->begin(); it != m->end(); it++) {
		LocalDataManagedValue* managedVal = it->second;

		std::string typeString = std::to_string(managedVal->type);

		std::string v;
		if(managedVal->type == LocalDataManagedValue::TYPE::T_bool) {
			bool b = *(static_cast<bool*>(managedVal->value));
			v = (!b) ? "false" : "true";
		}else if(managedVal->type == LocalDataManagedValue::TYPE::T_int) {
			int i = *(static_cast<int*>(managedVal->value));
			v = std::to_string(i);
		}else if(managedVal->type == LocalDataManagedValue::TYPE::T_String) {
			v = *static_cast<std::string *>(managedVal->value);
		}else if(managedVal->type == LocalDataManagedValue::TYPE::T_SharedPtr_String) {
			std::shared_ptr<std::string> spString = *static_cast<std::shared_ptr<std::string>*>(managedVal->value);
			v = *spString;
		}else {
			throw LogicException(LOC);
		}

		dataMapSS << it->first << ":" << typeString << "=" << v << std::endl;

		count++;
	}
	mtxDataMap->unlock();

	std::string dataMapString = dataMapSS.str();

	std::shared_ptr<IFileHandlerBlocking> fhTemp = Util_FileSystem::fs->getFileHandlerBlocking(DataPathParts::FOLDER + DataPathParts::FILE_TEMP, DataPathParts::FILE_SYSTEM_TARGET);
	std::shared_ptr<IFileHandlerBlocking> fhBackup = Util_FileSystem::fs->getFileHandlerBlocking(DataPathParts::FOLDER + DataPathParts::FILE_BACKUP, DataPathParts::FILE_SYSTEM_TARGET);
	std::shared_ptr<IFileHandlerBlocking> fhMain = Util_FileSystem::fs->getFileHandlerBlocking(DataPathParts::FOLDER + DataPathParts::FILE_MAIN, DataPathParts::FILE_SYSTEM_TARGET);

	fhTemp->createBoundedDirectories();
	fhTemp->write(dataMapString);

	fhBackup->deleteIfExists();

	try {
		fhMain->rename(DataPathParts::FILE_BACKUP);
	}catch(ResourceNotFoundException& x) {
		// THe main file is optional at this location.
		//void
	}
	try {
		fhTemp->rename(DataPathParts::FILE_MAIN);
	}catch(ResourceNotFoundException& x) {
		throw LogicException(LOC);
	}

	if(AppConfig::IS_DEBUG && IS_DEBUG) {
		std::cout << "Local profile data saved. Entries count: " << count << std::endl;
	}
}

void ProfileStorageBase::loadDataMap() {
	std::shared_ptr<IFileHandlerBlocking> fh = Util_FileSystem::fs->getFileHandlerBlocking(DataPathParts::FOLDER + DataPathParts::FILE_MAIN, DataPathParts::FILE_SYSTEM_TARGET);
	if(!fh->isExists()) {
		fh = Util_FileSystem::fs->getFileHandlerBlocking(DataPathParts::FOLDER + DataPathParts::FILE_BACKUP, DataPathParts::FILE_SYSTEM_TARGET);
	}

	if(!fh->isExists()) {
		// No profile data file found. Nothing to load.

		return ;
	}

	std::shared_ptr<base::String> fileData = fh->readStringSP();

	std::shared_ptr<ArrayList<std::string>> linesList = StringUtil::split2(fileData->str, "\n", "\r", false);
	int count = 0;
	for(int i=0;i<linesList->count();i++) {
		std::string& lineData = linesList->getReference(i);

		std::shared_ptr<ArrayList<std::string>> toupleList = StringUtil::split2(lineData, ":", "=", true);
		if(toupleList->size() != 3) {
			throw LogicException(LOC);
		}

		std::string& data_key = toupleList->getReference(0);
		std::string& data_typeS = toupleList->getReference(1);
		std::string& data_valueS = toupleList->getReference(2);
		int data_type = std::stoi(data_typeS);

		void* data_value;
		if(data_type == LocalDataManagedValue::TYPE::T_bool) {
			if(data_valueS == "false") {
				data_value = newt bool(false);
			}else if(data_valueS == "true") {
				data_value = newt bool(true);
			}else {
				throw LogicException(LOC);
			}
		}else if(data_type == LocalDataManagedValue::TYPE::T_int) {
			data_value = newt int(std::stoi(data_valueS));
		}else if(data_type == LocalDataManagedValue::TYPE::T_String) {
			data_value = newt std::string(data_valueS);
		}else if(data_type == LocalDataManagedValue::TYPE::T_SharedPtr_String) {
			std::shared_ptr<std::string> spString = std::make_shared<std::string>(data_valueS);

			data_value = newt std::shared_ptr<std::string>(spString);
		}else {
			throw LogicException(LOC);
		}

		LocalDataManagedValue* newDataEntry = newt LocalDataManagedValue(
			data_type,
			data_value
		);

        safePutEntry(
            data_key,
            newDataEntry,
            false
        );

		count++;
	}

	if(AppConfig::IS_DEBUG && IS_DEBUG) {
		std::cout << "Local profile data loaded. Entries count: " << count << std::endl;
	}
}

int ProfileStorageBase::getInt(std::string& key, int defaultValue) {
	if(!dataMap.containsKey(key)) {
		return defaultValue;
	}

	LocalDataManagedValue* v = dataMap.getDirect(key);

	return *static_cast<int*>(v->value);
}

void ProfileStorageBase::putInt(std::string& key, int value) {
	safePutEntry(
		key,
		new LocalDataManagedValue(
			LocalDataManagedValue::TYPE::T_int,
			new int(value)
		),
		true
	);
}

bool ProfileStorageBase::getBoolean(std::string& key, bool defaultValue) {
	if(!dataMap.containsKey(key)) {
		return defaultValue;
	}

	LocalDataManagedValue* v = dataMap.getDirect(key);

	return *static_cast<bool*>(v->value);
}

void ProfileStorageBase::putBoolean(std::string& key, bool value) {
	safePutEntry(
		key,
		new LocalDataManagedValue(
			LocalDataManagedValue::TYPE::T_bool,
			new bool(value)
		),
		true
	);
}

std::string* ProfileStorageBase::getString(std::string& key) {
	if(!dataMap.containsKey(key)) {
		return nullptr;
	}

	LocalDataManagedValue* v = dataMap.getDirect(key);

	return static_cast<std::string*>(v->value);
}

void ProfileStorageBase::putString(std::string& key, std::string& value) {
	safePutEntry(
		key,
		new LocalDataManagedValue(
			LocalDataManagedValue::TYPE::T_String,
			new std::string(value)
		),

		true
	);
}

std::shared_ptr<std::string> ProfileStorageBase::getSharedPtrString(std::string& key) {
	if(!dataMap.containsKey(key)) {
		return nullptr;
	}

	LocalDataManagedValue* v = dataMap.getDirect(key);

	return *(static_cast<std::shared_ptr<std::string>*>(v->value));
}

void ProfileStorageBase::putSharedPtrString(std::string& key, std::shared_ptr<std::string> value) {
	if(value == nullptr) {
		removeEntry(key);
	}else {
		safePutEntry(
			key,
			new LocalDataManagedValue(
				LocalDataManagedValue::TYPE::T_SharedPtr_String,
				new std::shared_ptr<std::string>(value)
			),

			true
		);
	}
}

void ProfileStorageBase::removeEntry(std::string& key) {
	LocalDataManagedValue* v = dataMap.getDirect(key, nullptr);
	if(v) {
		dataMap.removeMustExist(key);

		delete v;
	}
}

void ProfileStorageBase::clearData() {
	dataMap.clear();

	isSaveRequested = true;
}

void ProfileStorageBase::manuallySaveData() {
	isSaveRequested = true;
}

void ProfileStorageBase::safePutEntry(std::string& key, LocalDataManagedValue* managedValue, bool doSaveData) {
	removeEntry(key);

	mtxDataMap->lock();
	dataMap.putDirect(key, managedValue);
	mtxDataMap->unlock();

	if(doSaveData) {
		isSaveRequested = true;
	}
}

void ProfileStorageBase::disposeMain() {
	if (managedThread != nullptr) {
		managedThread->removeManaged();
		managedThread = nullptr;
	}

	super::disposeMain();
}

ProfileStorageBase::~ProfileStorageBase() {
	if(mtxDataMap != nullptr) {
		delete mtxDataMap;
		mtxDataMap = nullptr;
	}
}
