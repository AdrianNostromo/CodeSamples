#pragma once

#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>

namespace battlezones_a_v1 {
class EntityTemplatesList : public rpg3D::EntityTemplatesList {
private: typedef base::EntityTemplatesList super;
private:
	static std::string NAMESPACE;

public:
	static bool IsStaticsInitHooked;

    static EntityTemplate* Mob_General_A_V1;

	static EntityTemplate* Doodad_BrokenZoneTeleporter_Stone_A_V1;
	static EntityTemplate* Doodad_BrokenNexusTeleporter_Stone_A_V1;

	static EntityTemplate* Platform_ZoneTeleporter_Wood;
	static EntityTemplate* Platform_NexusTeleporter_Wood_A_V1;

public:
    static EntityTemplate* newEntry(std::string id);

private:
	static void InitStatics();

};
};
