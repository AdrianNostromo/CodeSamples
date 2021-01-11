#pragma once

#include <string>

namespace base {
class IPlayerProfileDataEventListener {
public:
    virtual void profileDataChanged(std::string& dataID) = 0;

    virtual ~IPlayerProfileDataEventListener() = default;
};
};
