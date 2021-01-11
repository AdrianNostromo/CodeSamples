#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <worldGame3D/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/config/ToolConfigBase.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/config/ToolConfigBase.h>

namespace rpg3D {
class EntityTemplatesList : public base::EntityTemplatesList {priv typedef base::EntityTemplatesList super;
	priv static std::string NAMESPACE;

	pub static bool IsStaticsInitHooked;

	pub static EntityTemplate* Unit_PlayerA;

	pub static EntityTemplate *Platform_TeleporterA;
	pub static EntityTemplate *Platform_MatchFindA;
	pub static EntityTemplate *Platform_BasicA;
	pub static EntityTemplate *Platform_PaintPad;
	pub static EntityTemplate *Platform_CreatureCreatorPad;
	pub static EntityTemplate *Platform_Teleporter_Home;
	pub static EntityTemplate *Platform_Teleporter_HomeSmall;

	pub static EntityTemplate *Doodad_BrokenTeleporter_Stone_A_V1;

	pub static EntityTemplate *Ground_BasicPatchA_1m;
	pub static EntityTemplate *Ground_BasicPatchA_6dm;

	pub static EntityTemplate *Flora_TreeA;
	pub static EntityTemplate *Flora_SmallFlower_A_V1;

	pub static EntityTemplate* Text_Painter;
	pub static EntityTemplate* Text_TeleporterRogueZone;

	pub static rpg3D::ToolConfigBase* Tool_ToolEEColorPicker;
	pub static rpg3D::ToolConfigBase* Tool_ToolEEPainter;
	pub static rpg3D::ToolConfigBase* Tool_ToolEEPlacer;
	pub static rpg3D::ToolConfigBase* Tool_ToolEERemoverLaser;
	pub static rpg3D::ToolConfigBase* Tool_ToolEERemoverSingle;

	pub static rpg3D::ToolConfigBase* Tool_ToolZEGroundTiled;
	pub static rpg3D::ToolConfigBase* Tool_ToolZEWallsFullTiled;
	pub static rpg3D::ToolConfigBase* Tool_ToolZEAssets;
	pub static rpg3D::ToolConfigBase* Tool_ToolZESun;

	pub static rpg3D::ToolConfigBase* Tool_StackablesHauler_A_V1;
	pub static const float CfgGeneral_Tool_StackablesHauler_GenericPickupRange;

	pub static EntityTemplate* newEntry(std::string id);

	priv static void InitStatics();

	priv static rpg3D::ToolConfigBase* new_Tool_ToolEEColorPicker();
	priv static rpg3D::ToolConfigBase* new_Tool_ToolEEPainter();
	priv static rpg3D::ToolConfigBase* new_Tool_ToolEEPlacer();
	priv static rpg3D::ToolConfigBase* new_Tool_ToolEERemoverLaser();
	priv static rpg3D::ToolConfigBase* new_Tool_ToolEERemoverSingle();

	priv static rpg3D::ToolConfigBase* new_Tool_ToolZEGroundTiled();
	priv static rpg3D::ToolConfigBase* new_Tool_ToolZEWallsFullTiled();
	priv static rpg3D::ToolConfigBase* new_Tool_ToolZEAssets();
	priv static rpg3D::ToolConfigBase* new_Tool_ToolZESun();

	priv static rpg3D::ToolConfigBase* new_Tool_StackablesHauler_A_V1();

};
};
