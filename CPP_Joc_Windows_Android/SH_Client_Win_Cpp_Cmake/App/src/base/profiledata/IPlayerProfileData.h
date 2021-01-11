#pragma once

#include <string>
#include <memory>

namespace base {
class IPlayerProfileDataEventListener;

class IPlayerProfileData {
public:
    virtual void addEventsListener(std::string& dataId, IPlayerProfileDataEventListener* eventsListener) = 0;
    virtual void removeEventsListener(std::string& dataId, IPlayerProfileDataEventListener* eventsListener) = 0;

    virtual int getDataWithID_int(std::string& dataId) = 0;
    virtual int getDataWithID_int(std::string& dataId, int defaultValue) = 0;
    virtual void putDataWithID_int(std::string& dataId, int value) = 0;
    virtual void addDataWithID_int(std::string& dataId, int addValue) = 0;
    virtual void putDataWithIDIfHigher_int(std::string& dataId, int value) = 0;

    virtual bool getDataWithID_bool(std::string& dataId, bool defaultValue) = 0;
    virtual bool getDataWithID_bool(std::string& dataId) = 0;
    virtual void putDataWithID_bool(std::string& dataId, bool value) = 0;

    virtual std::string* getDataWithID_String(std::string& dataId) = 0;
    virtual void putDataWithID_String(std::string& dataId, std::string& value) = 0;

    virtual std::shared_ptr<std::string> getDataWithID_SharedPtrString(std::string& dataId) = 0;
    virtual void putDataWithID_SharedPtrString(std::string& dataId, std::shared_ptr<std::string> value) = 0;

    virtual void removeDataWithID(std::string& dataId) = 0;

    virtual void clearAllData() = 0;
    virtual void sensitiveDataChanged() = 0;

    virtual ~IPlayerProfileData() = default;
};
};
