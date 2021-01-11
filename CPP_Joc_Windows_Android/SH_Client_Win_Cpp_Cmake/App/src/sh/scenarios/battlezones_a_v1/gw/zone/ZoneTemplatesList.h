#pragma once

#include "ZoneTemplate.h"

namespace battlezones_a_v1 {
class ZoneTemplatesList {
public:
	static bool IsStaticsInitHooked;

	static ZoneTemplate* Nexus_A_V1;

    static ZoneTemplate* Battle_WESN_A_V1;

private:
	static void InitStatics();

};
};
