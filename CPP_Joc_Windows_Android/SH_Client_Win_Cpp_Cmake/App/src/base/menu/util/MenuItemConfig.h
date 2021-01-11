#pragma once

#include <string>
#include <base/list/Array1D.h>
#include <memory>

class MenuItemConfig {
public:
    std::string itemID;

public:
    MenuItemConfig(MenuItemConfig const&) = delete;
    MenuItemConfig(MenuItemConfig &&) = default;
    MenuItemConfig& operator=(MenuItemConfig const&) = delete;
    MenuItemConfig& operator=(MenuItemConfig &&) = default;
    explicit MenuItemConfig(std::string itemID);

    virtual ~MenuItemConfig();
};
