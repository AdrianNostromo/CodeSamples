#pragma once

#include <base/log/ILogHandler.h>
#include <base/profilestorage/IProfileStorage.h>
#include <base/map/Map1D.h>
#include <base/list/ArrayList.h>
#include <base/profiledata/util/IPlayerProfileDataEventListener.h>
#include "IPlayerProfileData.h"

namespace base {
class PlayerProfileDataBase : virtual public IPlayerProfileData {
protected:
    ILogHandler* logger;

    IProfileStorage* localDataHandler;

    Map1D<std::string, ArrayList<IPlayerProfileDataEventListener*>*> eventListenersMap{};
public:
    PlayerProfileDataBase(PlayerProfileDataBase const&) = delete;
    PlayerProfileDataBase(PlayerProfileDataBase &&) = default;
    PlayerProfileDataBase& operator=(PlayerProfileDataBase const&) = delete;
    PlayerProfileDataBase& operator=(PlayerProfileDataBase &&) = default;

    explicit PlayerProfileDataBase(ILogHandler* logger, IProfileStorage* localDataHandler);

    void addEventsListener(std::string& dataId, IPlayerProfileDataEventListener* eventsListener) override;
    void removeEventsListener(std::string& dataId, IPlayerProfileDataEventListener* eventsListener) override;

    int getDataWithID_int(std::string& dataId) override;
    int getDataWithID_int(std::string& dataId, int defaultValue) override;
    void putDataWithID_int(std::string& dataId, int value) override;
    void addDataWithID_int(std::string& dataId, int addValue) override;
    void putDataWithIDIfHigher_int(std::string& dataId, int value) override;

    bool getDataWithID_bool(std::string& dataId, bool defaultValue) override;
    bool getDataWithID_bool(std::string& dataId) override;
    void putDataWithID_bool(std::string& dataId, bool value) override;

    std::string* getDataWithID_String(std::string& dataId) override;
    void putDataWithID_String(std::string& dataId, std::string& value) override;

    std::shared_ptr<std::string> getDataWithID_SharedPtrString(std::string& dataId) override;
    void putDataWithID_SharedPtrString(std::string& dataId, std::shared_ptr<std::string> value) override;

    void removeDataWithID(std::string& dataId) override;

    void clearAllData() override;
    void sensitiveDataChanged() override;

    ~PlayerProfileDataBase() override;
protected:
    void dispatchEvent_profileDataChanged(std::string& dataId);

};
};
