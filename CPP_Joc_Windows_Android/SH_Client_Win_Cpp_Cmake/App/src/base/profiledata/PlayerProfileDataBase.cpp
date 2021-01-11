#include "PlayerProfileDataBase.h"
#include <base/exceptions/LogicException.h>
#include <base/MM.h>

using namespace base;

PlayerProfileDataBase::PlayerProfileDataBase(ILogHandler* logger, IProfileStorage* localDataHandler)
    : logger(logger), localDataHandler(localDataHandler)
{
    //void
}

void PlayerProfileDataBase::addEventsListener(std::string& dataId, IPlayerProfileDataEventListener* eventsListener) {
    ArrayList<IPlayerProfileDataEventListener*>* eventListenersList = eventListenersMap.getDirect(dataId);

    if(eventListenersList != nullptr && eventListenersList->indexOf(eventsListener) >= 0) {
        throw LogicException(LOC);
    }

    if(eventListenersList == nullptr) {
        eventListenersList = new ArrayList<IPlayerProfileDataEventListener*>();
        eventListenersMap.putDirect(dataId, eventListenersList);
    }

    eventListenersList->appendDirect(eventsListener);
}

void PlayerProfileDataBase::removeEventsListener(std::string& dataId, IPlayerProfileDataEventListener* eventsListener) {
    ArrayList<IPlayerProfileDataEventListener*>* eventListenersList = eventListenersMap.getDirect(dataId);
    if(eventListenersList == nullptr) {
        throw LogicException(LOC);
    }

    int i = eventListenersList->indexOf(eventsListener);
    if(i < 0) {
        logger->postDebug("Events frustumCulling3DHandler remove error.");

        return ;
    }

    eventListenersList->remove(i);

    if(eventListenersList->size() <= 0) {
        delete eventListenersList;
        eventListenersMap.removeMustExist(dataId);
    }
}

int PlayerProfileDataBase::getDataWithID_int(std::string& dataId) {
    return getDataWithID_int(dataId, 0);
}

int PlayerProfileDataBase::getDataWithID_int(std::string& dataId, int defaultValue) {
    return localDataHandler->getInt(dataId, defaultValue);
}

void PlayerProfileDataBase::putDataWithID_int(std::string& dataId, int value) {
    localDataHandler->putInt(dataId, value);

    dispatchEvent_profileDataChanged(dataId);
}

void PlayerProfileDataBase::addDataWithID_int(std::string& dataId, int addValue) {
    putDataWithID_int(dataId, getDataWithID_int(dataId) + addValue);
}

void PlayerProfileDataBase::putDataWithIDIfHigher_int(std::string& dataId, int value) {
    if(value > getDataWithID_int(dataId)) {
        putDataWithID_int(dataId, value);
    }
}

bool PlayerProfileDataBase::getDataWithID_bool(std::string& dataId, bool defaultValue) {
    return localDataHandler->getBoolean(dataId, defaultValue);
}

bool PlayerProfileDataBase::getDataWithID_bool(std::string& dataId) {
    return getDataWithID_bool(dataId, false);
}

void PlayerProfileDataBase::putDataWithID_bool(std::string& dataId, bool value) {
    localDataHandler->putBoolean(dataId, value);

    dispatchEvent_profileDataChanged(dataId);
}

std::string* PlayerProfileDataBase::getDataWithID_String(std::string& dataId) {
    return localDataHandler->getString(dataId);
}

void PlayerProfileDataBase::putDataWithID_String(std::string& dataId, std::string& value) {
    localDataHandler->putString(dataId, value);

    dispatchEvent_profileDataChanged(dataId);
}

std::shared_ptr<std::string> PlayerProfileDataBase::getDataWithID_SharedPtrString(std::string& dataId) {
    return localDataHandler->getSharedPtrString(dataId);
}

void PlayerProfileDataBase::putDataWithID_SharedPtrString(std::string& dataId, std::shared_ptr<std::string> value) {
    localDataHandler->putSharedPtrString(dataId, value);

    dispatchEvent_profileDataChanged(dataId);
}

void PlayerProfileDataBase::removeDataWithID(std::string& dataId) {
    localDataHandler->removeEntry(dataId);

    dispatchEvent_profileDataChanged(dataId);
}

void PlayerProfileDataBase::clearAllData() {
    localDataHandler->clearData();
}

void PlayerProfileDataBase::sensitiveDataChanged() {
    // Do a manual save to override the backup.
    localDataHandler->manuallySaveData();
}

void PlayerProfileDataBase::dispatchEvent_profileDataChanged(std::string& dataId) {
    ArrayList<IPlayerProfileDataEventListener*>* eventListenersList = eventListenersMap.getDirect(dataId, nullptr);

    if(eventListenersList != nullptr) {
        for (int i = 0; i < eventListenersList->size(); i++) {
            IPlayerProfileDataEventListener* entry = eventListenersList->getDirect(i);
            if (entry != nullptr) {
                entry->profileDataChanged(dataId);
            }
        }
    }
}

PlayerProfileDataBase::~PlayerProfileDataBase() {
    //void
}
