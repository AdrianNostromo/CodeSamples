#pragma once

#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>

namespace startarea_a_v1 {
class EntityTemplatesList : public rpg3D::EntityTemplatesList {
private: typedef rpg3D::EntityTemplatesList super;
private:
	static std::string NAMESPACE;

public:
	static bool IsStaticsInitHooked;

    static EntityTemplate* Platform_TeleporterA;
    static EntityTemplate* Platform_TeleporterDevA;
    static EntityTemplate* Platform_MatchFindA;
    static EntityTemplate* Platform_BasicA;
    static EntityTemplate* Platform_PaintPad;

    static EntityTemplate* Flora_TreeA;

    static EntityTemplate* Doodad_PlayerSpawnIndicatorA;

public:
    static EntityTemplate* newEntry(std::string id);

private:
	static void InitStatics();

};
};
