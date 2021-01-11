#pragma once

#include <string>

namespace rpg3D {
class ToolSlotConfig {
public:
	std::string toolHolderVisualId;

public:
	ToolSlotConfig(ToolSlotConfig const&) = delete;
	ToolSlotConfig(ToolSlotConfig &&) = default;
	ToolSlotConfig& operator=(ToolSlotConfig const&) = delete;
	ToolSlotConfig& operator=(ToolSlotConfig &&) = default;

	explicit ToolSlotConfig(std::string toolHolderVisualId);

	virtual ~ToolSlotConfig();
};
};
