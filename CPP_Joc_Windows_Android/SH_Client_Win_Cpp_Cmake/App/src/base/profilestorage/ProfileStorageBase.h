#pragma once

#include <base/gh.h>
#include <base/profilestorage/IProfileStorage.h>
#include <base/map/Map1D.h>
#include <mutex>
#include <base/object/ManagedObject.h>

class IApp;
namespace base {
	class IManagedThread;
};

namespace base {
class ProfileStorageBase : public ManagedObject, virtual public base::IProfileStorage {priv typedef ManagedObject super;pub dCtor(ProfileStorageBase);
	priv class DataPathParts {
		pub static const int FILE_SYSTEM_TARGET;

		pub static const std::string FOLDER;

		pub static const std::string FILE_MAIN;
		pub static const std::string FILE_TEMP;
		pub static const std::string FILE_BACKUP;
	};

	priv static const bool IS_DEBUG = false;

	prot IApp* app = nullptr;

	prot bool isSaveRequested = false;

	priv base::IManagedThread* managedThread = nullptr;

	priv Map1D<std::string, LocalDataManagedValue*> dataMap{};
	priv std::mutex* mtxDataMap;

	pub explicit ProfileStorageBase();
	pub void init(IApp* app) final;
	prot void createMain() override;

	prot unsigned int run(base::IManagedThread* managedThread);

	pub int getInt(std::string& key, int defaultValue) override;
	pub void putInt(std::string& key, int value) override;

	pub bool getBoolean(std::string& key, bool defaultValue) override;
	pub void putBoolean(std::string& key, bool value) override;

	pub std::string* getString(std::string& key) override;
	pub void putString(std::string& key, std::string& value) override;

	pub std::shared_ptr<std::string> getSharedPtrString(std::string& key) override;
	pub void putSharedPtrString(std::string& key, std::shared_ptr<std::string> value) override;

	pub void removeEntry(std::string& key) override;
	pub void clearData() override;

	pub void manuallySaveData() override;

	pub bool flush() final;

	priv void safePutEntry(std::string& key, LocalDataManagedValue* managedValue, bool doSaveData);

	priv void saveDataMap();
	pub void loadDataMap() final;

	prot void disposeMain() override;
	pub ~ProfileStorageBase() override;
};
};
