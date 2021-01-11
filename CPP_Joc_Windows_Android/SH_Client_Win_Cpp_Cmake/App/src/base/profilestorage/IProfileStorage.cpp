#include "IProfileStorage.h"
#include <base/exceptions/LogicException.h>
#include <memory>
#include <assert.h>

using namespace base;

const int IProfileStorage::LocalDataManagedValue::TYPE::T_bool = 1;
const int IProfileStorage::LocalDataManagedValue::TYPE::T_int = 2;
const int IProfileStorage::LocalDataManagedValue::TYPE::T_String = 3;
const int IProfileStorage::LocalDataManagedValue::TYPE::T_SharedPtr_String = 4;

IProfileStorage::LocalDataManagedValue::LocalDataManagedValue(const int type, void* value)
    : type(type), value(value)
{
    //void
}

IProfileStorage::LocalDataManagedValue::~LocalDataManagedValue() {
    if(value) {
        if(type == TYPE::T_bool) {
            bool* v = static_cast<bool*>(value);
            delete v;
        }else if(type == TYPE::T_int) {
            int* v = static_cast<int*>(value);
            delete v;
        }else if(type == TYPE::T_String) {
            std::string* v = static_cast<std::string*>(value);
            delete v;
        }else if(type == TYPE::T_SharedPtr_String) {
            std::shared_ptr<std::string>* v = static_cast<std::shared_ptr<std::string>*>(value);
            delete v;
        }else {
            assert(false);
        }

        value = nullptr;
    }
}
