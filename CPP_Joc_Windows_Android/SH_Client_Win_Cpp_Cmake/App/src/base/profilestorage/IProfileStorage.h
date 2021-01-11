#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include <base/object/IManagedObject.h>

class IApp;

namespace base {
class IProfileStorage : virtual public IManagedObject {
    pub class LocalDataManagedValue {pub dCtor(LocalDataManagedValue);
        pub class TYPE {
            pub static const int T_bool;
            pub static const int T_int;
            pub static const int T_String;
            pub static const int T_SharedPtr_String;
        };
        
        pub int type;

        pub void* value;

        pub explicit LocalDataManagedValue(const int type, void* value);

        pub virtual ~LocalDataManagedValue();
    };

public:
    virtual void init(IApp* app) = 0;

    virtual int getInt(std::string& key, int defaultValue) = 0;
    virtual void putInt(std::string& key, int value) = 0;

    virtual bool getBoolean(std::string& key, bool defaultValue) = 0;
    virtual void putBoolean(std::string& key, bool value) = 0;

    virtual std::string* getString(std::string& key) = 0;
    virtual void putString(std::string& key, std::string& value) = 0;

    virtual std::shared_ptr<std::string> getSharedPtrString(std::string& key) = 0;
    virtual void putSharedPtrString(std::string& key, std::shared_ptr<std::string> value) = 0;

    virtual void removeEntry(std::string& key) = 0;
    virtual void clearData() = 0;

    // This is used after a logout to remove the backup file that contained the account information.
    virtual void manuallySaveData() = 0;

    pub virtual void loadDataMap() = 0;

    pub virtual bool flush() = 0;

    virtual ~IProfileStorage() = default;
};
};
