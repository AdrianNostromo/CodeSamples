#pragma once

#include <base/math/Vector3.h>
#include "ZoneTemplate.h"

namespace battlezones_a_v1 {
class ZoneTemplate_Battle_WESN_A_V1 {
public:
	static std::string FLAG_isNextZoneNexus;
	static std::string FLAG_isPreviousZoneNexus;
	static std::string FLAG_isZoneCleared;

	static ZoneTemplate* getNew();

};
};
