#include "EntityTemplatesList.h"
#include <rpg3D/gw/config/entity/unit/UnitTemplates_PlayerA.h>
#include <rpg3D/gw/config/entity/platform/PlatformTemplates_TeleporterA.h>
#include <rpg3D/gw/config/entity/platform/PlatformTemplates_MatchFindA.h>
#include <rpg3D/gw/config/entity/platform/PlatformTemplates_BasicA.h>
#include <rpg3D/gw/config/entity/platform/PlatformTemplates_PaintPad.h>
#include <rpg3D/gw/config/entity/platform/EntityTemplates_Platform_CreatureCreatorPad.h>
#include <rpg3D/gw/config/entity/platform/EntityTemplates_Platform_Teleporter_Home.h>
#include <rpg3D/gw/config/entity/platform/EntityTemplates_Platform_Teleporter_HomeSmall.h>
#include <rpg3D/gw/config/entity/ground/GroundTemplates_BasicPatchA_1m.h>
#include <rpg3D/gw/config/entity/ground/GroundTemplates_BasicPatchA_6dm.h>
#include <rpg3D/gw/config/entity/flora/FloraTemplates_TreeA.h>
#include <rpg3D/gw/config/entity/flora/Template_Flora_SmallFlower_A_V1.h>
#include <rpg3D/gw/config/entity/doodad/Template_Doodad_BrokenTeleporter_Stone_A_V1.h>
#include <rpg3D/gw/config/entity/text/TextTemplate_TeleporterPainter.h>
#include <rpg3D/gw/config/entity/text/TextTemplate_TeleporterRogueZone.h>
#include <base/statics/StaticsInit.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/colorPicker/config/ToolConfigEEColorPicker.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/painter/config/ToolConfigEEPainter.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/placer/config/ToolConfigEEPlacer.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/removerLaser/config/ToolConfigEERemoverLaser.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/removerSingle/config/ToolConfigEERemoverSingle.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/zoneEditor/groundTiled/config/ToolConfigZEGroundTiled.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/zoneEditor/wallsFullTiled/config/ToolConfigZEWallsFullTiled.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/zoneEditor/assets/config/ToolConfigZEAssets.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/zoneEditor/sun/config/ToolConfigZESun.h>

#include <rpg3D/gw/entity/module/toolsHandler/tool/stackablesHauler/config/ToolConfigStackablesHauler.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <rpg3D/gw/config/entity/projectile/T_Projectile_Arrow_Basic_A_V1.h>
#include <rpg3D/gw/config/entity/projectile/T_Projectile_Arrow_Basic_Grid06Range3Tiles_A_V1.h>
#include <rpg3D/gw/config/entity/projectile/T_Projectile_Bolt_Basic_A_V1.h>
#include <rpg3D/gw/config/entity/projectile/T_Projectile_Bullet_Basic_A_V1.h>
#include <rpg3D/gw/config/entity/projectile/T_Projectile_LaserSegment_Basic_A_V1.h>

#include <rpg3D/gw/config/entity/tool/Cfg_Tool_AutomaticRifle_A_V1.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_BallAndChain_A_V1.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_Bow_Basic_A_V1.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_ChickenBeak_A_V1.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_Crossbow_Basic_A_V1.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_HandgunAutomatic_A_V1.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_HandgunManual_A_V1.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_LaserGun_A_V1.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_Mace_A_V1.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_Shotgun_A_V1.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_Spear_A_V1.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_SubMachineGun_A_V1.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_Sword_Basic_A_V1.h>

#include <rpg3D/gw/config/entity/structure/TC_GroundOrWall.h>
#include <rpg3D/gw/config/entity/doodad/TC_Bed_A_V1.h>

#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/custom/T_UnitValidatorHaulerPlatform.h>
#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/platform/T_PlatformSlimeTeleporter.h>
#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/environment/T_Doorframe_A_V1.h>
#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/environment/T_Pillar_A_V1.h>
#include <sh/scenarios/squareTactics_a_v1/gw/config/entity/environment/T_WallElement_A_V1.h>

using namespace rpg3D;

std::string EntityTemplatesList::NAMESPACE = "rpg3D";

bool EntityTemplatesList::IsStaticsInitHooked = base::StaticsInit::AddCbGeneral<bool>(1, EntityTemplatesList::InitStatics);

EntityTemplate* EntityTemplatesList::Unit_PlayerA = nullptr;

EntityTemplate* EntityTemplatesList::Platform_TeleporterA = nullptr;
EntityTemplate* EntityTemplatesList::Platform_MatchFindA = nullptr;
EntityTemplate* EntityTemplatesList::Platform_BasicA = nullptr;
EntityTemplate* EntityTemplatesList::Platform_PaintPad = nullptr;
EntityTemplate* EntityTemplatesList::Platform_CreatureCreatorPad = nullptr;
EntityTemplate* EntityTemplatesList::Platform_Teleporter_Home = nullptr;
EntityTemplate* EntityTemplatesList::Platform_Teleporter_HomeSmall = nullptr;

EntityTemplate* EntityTemplatesList::Doodad_BrokenTeleporter_Stone_A_V1 = nullptr;

EntityTemplate* EntityTemplatesList::Ground_BasicPatchA_1m = nullptr;
EntityTemplate* EntityTemplatesList::Ground_BasicPatchA_6dm = nullptr;

EntityTemplate* EntityTemplatesList::Flora_TreeA = nullptr;
EntityTemplate* EntityTemplatesList::Flora_SmallFlower_A_V1 = nullptr;

EntityTemplate* EntityTemplatesList::Text_Painter = nullptr;
EntityTemplate* EntityTemplatesList::Text_TeleporterRogueZone = nullptr;

rpg3D::ToolConfigBase* EntityTemplatesList::Tool_ToolEEColorPicker = nullptr;
rpg3D::ToolConfigBase* EntityTemplatesList::Tool_ToolEEPainter = nullptr;
rpg3D::ToolConfigBase* EntityTemplatesList::Tool_ToolEEPlacer = nullptr;
rpg3D::ToolConfigBase* EntityTemplatesList::Tool_ToolEERemoverLaser = nullptr;
rpg3D::ToolConfigBase* EntityTemplatesList::Tool_ToolEERemoverSingle = nullptr;

rpg3D::ToolConfigBase* EntityTemplatesList::Tool_ToolZEGroundTiled = nullptr;
rpg3D::ToolConfigBase* EntityTemplatesList::Tool_ToolZEWallsFullTiled = nullptr;
rpg3D::ToolConfigBase* EntityTemplatesList::Tool_ToolZEAssets = nullptr;
rpg3D::ToolConfigBase* EntityTemplatesList::Tool_ToolZESun = nullptr;

rpg3D::ToolConfigBase* EntityTemplatesList::Tool_StackablesHauler_A_V1 = nullptr;
const float EntityTemplatesList::CfgGeneral_Tool_StackablesHauler_GenericPickupRange = 0.5f;

EntityTemplate* EntityTemplatesList::newEntry(std::string id) {
    return EntityTemplate::newEntry(NAMESPACE, id);
}

void EntityTemplatesList::InitStatics() {
	// This is used to make sure the statis are initialised for the classes because they may be used only through EntityTemplate.
	// This should be enough unless it gets optimised away.
	(void)TC_GroundOrWall::getOrCreateTemplate;
	(void)TC_Bed_A_V1::GetOrCreateTemplate;

    Unit_PlayerA = UnitTemplates_PlayerA::GetNew();

    Platform_TeleporterA = PlatformTemplates_TeleporterA::getNew();
    Platform_MatchFindA = PlatformTemplates_MatchFindA::getNew();
    Platform_BasicA = PlatformTemplates_BasicA::getNew();
    Platform_PaintPad = PlatformTemplates_PaintPad::getNew();
	Platform_CreatureCreatorPad = EntityTemplates_Platform_CreatureCreatorPad::getNew();
	Platform_Teleporter_HomeSmall = EntityTemplates_Platform_Teleporter_HomeSmall::getNew();
	Platform_Teleporter_Home = EntityTemplates_Platform_Teleporter_Home::getNew();

    Doodad_BrokenTeleporter_Stone_A_V1 = Template_Doodad_BrokenTeleporter_Stone_A_V1::getNew();

    Ground_BasicPatchA_1m = GroundTemplates_BasicPatchA_1m::getNew();
    Ground_BasicPatchA_6dm = GroundTemplates_BasicPatchA_6dm::getNew();

    Flora_TreeA = FloraTemplates_TreeA::getNew();
	Flora_SmallFlower_A_V1 = Template_Flora_SmallFlower_A_V1::getNew();

    Text_Painter = TextTemplate_TeleporterPainter::getNew();
    Text_TeleporterRogueZone = TextTemplate_TeleporterRogueZone::getNew();

	Tool_ToolEEColorPicker = new_Tool_ToolEEColorPicker();
	Tool_ToolEEPainter = new_Tool_ToolEEPainter();
	Tool_ToolEEPlacer = new_Tool_ToolEEPlacer();
	Tool_ToolEERemoverLaser = new_Tool_ToolEERemoverLaser();
	Tool_ToolEERemoverSingle = new_Tool_ToolEERemoverSingle();

	Tool_ToolZEGroundTiled = new_Tool_ToolZEGroundTiled();
	Tool_ToolZEWallsFullTiled = new_Tool_ToolZEWallsFullTiled();
	Tool_ToolZEAssets = new_Tool_ToolZEAssets();
	Tool_ToolZESun = new_Tool_ToolZESun();

	Tool_StackablesHauler_A_V1 = new_Tool_StackablesHauler_A_V1();

	Cfg_Tool_AutomaticRifle_A_V1::getOrCreateTemplate();
	Cfg_Tool_BallAndChain_A_V1::getOrCreateTemplate();
	Cfg_Tool_Bow_Basic_A_V1::getOrCreateTemplate();
	Cfg_Tool_ChickenBeak_A_V1::getOrCreateTemplate();
	Cfg_Tool_Crossbow_Basic_A_V1::getOrCreateTemplate();
	Cfg_Tool_HandgunAutomatic_A_V1::getOrCreateTemplate();
	Cfg_Tool_HandgunManual_A_V1::getOrCreateTemplate();
	Cfg_Tool_LaserGun_A_V1::getOrCreateTemplate();
	Cfg_Tool_Mace_A_V1::getOrCreateTemplate();
	Cfg_Tool_Shotgun_A_V1::getOrCreateTemplate();
	Cfg_Tool_Spear_A_V1::getOrCreateTemplate();
	Cfg_Tool_SubMachineGun_A_V1::getOrCreateTemplate();
	Cfg_Tool_Sword_Basic_A_V1::getOrCreateTemplate();

	T_Projectile_Arrow_Basic_A_V1::getOrCreateTemplate();
	T_Projectile_Arrow_Basic_Grid06Range3Tiles_A_V1::getOrCreateTemplate();
	T_Projectile_Bolt_Basic_A_V1::getOrCreateTemplate();
	T_Projectile_Bullet_Basic_A_V1::getOrCreateTemplate();
	T_Projectile_LaserSegment_Basic_A_V1::getOrCreateTemplate();
}

rpg3D::ToolConfigBase* EntityTemplatesList::new_Tool_ToolEEColorPicker() {
	rpg3D::ToolConfigEEColorPicker* t = new rpg3D::ToolConfigEEColorPicker(
		"Tool_ToolEEColorPicker",
		rpg3D::ToolConfigBase::FiltersPriority::manual,
		std::make_shared<std::string>("visual2D/taGeneral.atlas"), std::make_shared<std::string>("sh/tools/entityEditor/colorPicker"),
		"Color"/*shortName_partA*/, "Pick"/*shortName_partB*/,
		0.01f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		&SoundConfigsList::EntityEditor_ToolActivate_A_V1->managedPath->path, 0.25f
	);

	return t;
}

rpg3D::ToolConfigBase* EntityTemplatesList::new_Tool_ToolEEPainter() {
	rpg3D::ToolConfigEEPainter* t = new rpg3D::ToolConfigEEPainter(
		"Tool_ToolEEPainter",
		rpg3D::ToolConfigBase::FiltersPriority::manual,
		std::make_shared<std::string>("visual2D/taGeneral.atlas"), std::make_shared<std::string>("sh/tools/entityEditor/paint"),
		"Paint"/*shortName_partA*/, ""/*shortName_partB*/,
		0.01f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		&SoundConfigsList::EntityEditor_ToolActivate_A_V1->managedPath->path, 0.25f
	);

	return t;
}

rpg3D::ToolConfigBase* EntityTemplatesList::new_Tool_ToolEEPlacer() {
	rpg3D::ToolConfigEEPlacer* t = new rpg3D::ToolConfigEEPlacer(
		"Tool_ToolEEPlacer",
		rpg3D::ToolConfigBase::FiltersPriority::manual,
		std::make_shared<std::string>("visual2D/taGeneral.atlas"), std::make_shared<std::string>("sh/tools/entityEditor/place"),
		"Place"/*shortName_partA*/, ""/*shortName_partB*/,
		0.01f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		&SoundConfigsList::EntityEditor_ToolActivate_A_V1->managedPath->path, 0.25f
	);

	return t;
}

rpg3D::ToolConfigBase* EntityTemplatesList::new_Tool_ToolEERemoverLaser() {
	rpg3D::ToolConfigEERemoverLaser* t = new rpg3D::ToolConfigEERemoverLaser(
		"Tool_ToolEERemoverLaser",
		rpg3D::ToolConfigBase::FiltersPriority::manual,
		std::make_shared<std::string>("visual2D/taGeneral.atlas"), std::make_shared<std::string>("sh/tools/entityEditor/removeLaser"),
		"Remove"/*shortName_partA*/, "Laser"/*shortName_partB*/,
		0.01f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		&SoundConfigsList::EntityEditor_ToolActivate_A_V1->managedPath->path, 0.25f
	);

	return t;
}

rpg3D::ToolConfigBase* EntityTemplatesList::new_Tool_ToolEERemoverSingle() {
	rpg3D::ToolConfigEERemoverSingle* t = new rpg3D::ToolConfigEERemoverSingle(
		"Tool_ToolEERemoverSingle",
		rpg3D::ToolConfigBase::FiltersPriority::manual,
		std::make_shared<std::string>("visual2D/taGeneral.atlas"), std::make_shared<std::string>("sh/tools/entityEditor/removeSingle"),
		"Remove"/*shortName_partA*/, "Single"/*shortName_partB*/,
		0.01f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		&SoundConfigsList::EntityEditor_ToolActivate_A_V1->managedPath->path, 0.25f
	);

	return t;
}

rpg3D::ToolConfigBase* EntityTemplatesList::new_Tool_ToolZEGroundTiled() {
	rpg3D::ToolConfigZEGroundTiled* t = new rpg3D::ToolConfigZEGroundTiled(
		"Tool_ToolZEGroundTiled",
		rpg3D::ToolConfigBase::FiltersPriority::manual,
		std::make_shared<std::string>("visual2D/taGeneral.atlas"), std::make_shared<std::string>("rpg3D/gw/zoneEditor/tools/groundTiled"),
		"Ground"/*shortName_partA*/, "Tiles"/*shortName_partB*/,
		0.1f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		&SoundConfigsList::ZoneEditor_ToolActivate_A_V1->managedPath->path, 0.25f
	);

	return t;
}

rpg3D::ToolConfigBase* EntityTemplatesList::new_Tool_ToolZEWallsFullTiled() {
	rpg3D::ToolConfigZEWallsFullTiled* t = new rpg3D::ToolConfigZEWallsFullTiled(
		"Tool_ToolZEWallsFullTiled",
		rpg3D::ToolConfigBase::FiltersPriority::manual,
		std::make_shared<std::string>("visual2D/taGeneral.atlas"), std::make_shared<std::string>("rpg3D/gw/zoneEditor/tools/wallFullTiled"),
		"Wall"/*shortName_partA*/, "FullTiless"/*shortName_partB*/,
		0.1f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		&SoundConfigsList::ZoneEditor_ToolActivate_A_V1->managedPath->path, 0.25f
	);

	return t;
}

rpg3D::ToolConfigBase* EntityTemplatesList::new_Tool_ToolZEAssets() {
	rpg3D::ToolConfigZEAssets* t = new rpg3D::ToolConfigZEAssets(
		"Tool_ToolZEAssets",
		rpg3D::ToolConfigBase::FiltersPriority::manual,
		std::make_shared<std::string>("visual2D/taGeneral.atlas"), std::make_shared<std::string>("rpg3D/gw/zoneEditor/tools/assets"),
		"Assets"/*shortName_partA*/, ""/*shortName_partB*/,
		0.1f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		&SoundConfigsList::ZoneEditor_ToolActivate_A_V1->managedPath->path, 0.25f
	);

	return t;
}

rpg3D::ToolConfigBase* EntityTemplatesList::new_Tool_ToolZESun() {
	rpg3D::ToolConfigZESun* t = new rpg3D::ToolConfigZESun(
		"Tool_ToolZESun",
		rpg3D::ToolConfigBase::FiltersPriority::manual,
		std::make_shared<std::string>("visual2D/taGeneral.atlas"), std::make_shared<std::string>("rpg3D/gw/zoneEditor/tools/sun"),
		"Sun"/*shortName_partA*/, ""/*shortName_partB*/,
		0.1f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		&SoundConfigsList::ZoneEditor_ToolActivate_A_V1->managedPath->path, 0.25f
	);

	return t;
}

rpg3D::ToolConfigBase* EntityTemplatesList::new_Tool_StackablesHauler_A_V1() {
	rpg3D::ToolConfigStackablesHauler* t = new rpg3D::ToolConfigStackablesHauler(
		"Tool_StackablesHauler_A_V1",
		rpg3D::ToolConfigBase::FiltersPriority::manual,
		std::make_shared<std::string>("visual2D/taGeneral.atlas"), std::make_shared<std::string>("sh/tools/stackable"),
		"Hauler"/*shortName_partA*/, ""/*shortName_partB*/,
		0.05f/*cooldownS*/, 0.0f/*channelingDurationS*/,
		CfgGeneral_Tool_StackablesHauler_GenericPickupRange/*pickupRangeM*/, 0.5f/*throwRangeM*/,
		&SoundConfigsList::StackablesHauler_Activate_A_V1->managedPath->path, 0.25f,
		0.0f/*throwPosXYGridSize*/, 0.0f/*throwAngDegZGridSize*/
	);

	return t;
}
