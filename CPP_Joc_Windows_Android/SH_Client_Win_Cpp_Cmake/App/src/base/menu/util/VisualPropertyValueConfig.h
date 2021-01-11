#pragma once

#include <string>
#include "VisualValueConfig.h"

class VisualPropertyValueConfig {
public:
    std::string property;

    VisualValueConfig* smartValue;

public:
    VisualPropertyValueConfig(VisualPropertyValueConfig const&) = delete;
    VisualPropertyValueConfig(VisualPropertyValueConfig &&) = default;
    VisualPropertyValueConfig& operator=(VisualPropertyValueConfig const&) = delete;
    VisualPropertyValueConfig& operator=(VisualPropertyValueConfig &&) = default;

    explicit VisualPropertyValueConfig(std::string property, VisualValueConfig* smartValue);

    virtual ~VisualPropertyValueConfig();
};
