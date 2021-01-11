#include "Template_Turret_CubesCannon_A_V1.h"
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/math/Bounds2D.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/towerDefense_a_v1/Namespace.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <base/statics/StaticsInit.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/visual3d/util/Drawable3DCreateConfig_VoxelGrid.h>
#include <sh/scenarios/towerDefense_a_v1/gw/entity/template/customCubesCannonTurret/CustomCubesCannonTurretTemplate.h>
#include <rpg3D/gw/entity/template/controller/ControllerTemplate.h>
#include <rpg3D/gw/entity/template/statusEffects/StatusEffectsTemplate.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include <rpg3D/gw/entity/template/TeamTemplate.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/sounds/SoundConfigsList.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>

using namespace towerDefense_a_v1;

std::string Template_Turret_CubesCannon_A_V1::ID = "Turret_CubesCannon_A_V1";

EntityTemplate* Template_Turret_CubesCannon_A_V1::TEMPLATE = base::StaticsInit::AddCbGeneral<EntityTemplate*>(1, []() {
	EntityTemplate* t = TEMPLATE = EntityTemplate::newEntry(Namespace::NAMESPACE, ID);

	t->tagsList.appendReference(rpg3D::EntityTags::TURRET);

	t->setModuleTemplate(new Visual3DTemplate(
		new Visual3DCreateConfig(
		(new ArrayList<ArrayList<Drawable3DCreateConfigBase*>*>())
			->appendDirect_chain((new ArrayList<Drawable3DCreateConfigBase*>())
				->appendDirect_chain(new Drawable3DCreateConfig_Group(
					"_ROOT_",
					nullptr,
					nullptr,
					nullptr,
					nullptr
				))
				->appendDirect_chain(new Drawable3DCreateConfig_Group(
					"_Content_",
					new std::string("_ROOT_"),//parentId
					nullptr,
					nullptr,
					new Vector3(0.6f)
				))
				->appendDirect_chain(buildBase())
				->appendDirect_chain(buildTurret())
				->appendDirect_chain(buildBarrel())
				->appendDirect_chain(buildProjectile())
			)
		),
		false,//requiresHalfGridOffsetX
		false//requiresHalfGridOffsetY
	));

	t->setModuleTemplate(FrustumCullable3DTemplate::newOfsetedZSphere(
		0.5f,
		Math::dist(0.0f, 0.0f, 0.5f, 0.5f)
	));

	t->setModuleTemplate(new worldGame3D::ActionsTemplate(
		//void
	));

	t->setModuleTemplate(new CustomCubesCannonTurretTemplate(
		360.0f/*turretTurnSpeedZD*/, 135.0f/*barrelTurnSpeedYD*/,
		"_GroundBase_",
		"_Turret_",
		"_Barrel_",
		"_Projectile_",

		0.25f/*barrelPosZ_normal*/, 0.05f/*barrelPosZ_loading*/, 180.0f/*barrelRotY_loading*/,
		new Array1D<float>(4, new float[4]{ 0.0f, 90.0f, 180.0f, -90.0f })/*turretReloadAnglesZ*/,

		&SoundConfigsList::CannonTurretFire_A_V1->managedPath->path, 0.25f/*activationSoundConfig_volume*/
	));

	t->setModuleTemplate(new rpg3D::TeamTemplate(
		//void
	));

	t->setModuleTemplate(new rpg3D::ControllerTemplate(
		//void
	));

	t->setModuleTemplate(new rpg3D::StatusEffectsTemplate(
		//void
	));

	t->setModuleTemplate(new Touchable3DTemplate(
		Touchable3DTemplate::ShapeType::AABB,
		Vector3(
			-0.2f,
			-0.2f,
			0.0f
		),
		Vector3(
			0.2f,
			0.2f,
			0.4f
		)
	));

	t->setModuleTemplate(new rpg3D::StackableTemplate(
		false/*canHaul*/, true,/*canBeCarried*/
		0.4f,
		true/*allowPlaceOnThis*/
	));
});

Drawable3DCreateConfig_VoxelGrid* Template_Turret_CubesCannon_A_V1::buildBase() {
	const signed char O = -1;
	const signed char M = 0; // Metal
	const signed char P = 1; // Projectile

	Array3D<signed char>* voxelGrid = new Array3D<signed char>(
		8, 8, 1,
		false, true, true,
		new signed char[8 * 8 * 1]{
			O,M,M,M,M,M,M,O,
			M,M,M,M,M,M,M,M,
			M,M,M,M,M,M,M,M,
			M,M,M,P,P,M,M,M,
			M,M,M,P,P,M,M,M,
			M,M,M,M,M,M,M,M,
			M,M,M,M,M,M,M,M,
			O,M,M,M,M,M,M,O
		}
	);
	
	ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#E7E7E7FF")));
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#2BE4E7FF")));

	return new Drawable3DCreateConfig_VoxelGrid(
		"_GroundBase_",//id
		new std::string("_Content_"),//parentId

		new Vector3(0.0f, 0.0f, 0.0f),// pos
		nullptr,//rot
		nullptr,// scale

		0.1f/*voxelSize*/,
		// The z pos is manually set so the entity sits 0.5f under the ground.
		new Vector3(-8.0f / 2.0f + 0.5f, -8.0f / 2.0f + 0.5f, 0.5f),

		voxelGrid, nullptr,
		materialsList, nullptr
	);
}

Drawable3DCreateConfig_VoxelGrid* Template_Turret_CubesCannon_A_V1::buildTurret() {
	const signed char O = -1;
	const signed char M = 0; // Metal

	Array3D<signed char>* voxelGrid = new Array3D<signed char>(
		6, 6, 3,
		false, true, true,
		new signed char[6 * 6 * 3]{
			O,O,M,M,O,O,
			O,O,O,O,O,O,
			O,O,O,O,O,O,
			O,O,O,O,O,O,
			O,O,O,O,O,O,
			O,O,M,M,O,O,
			
			O,O,M,M,O,O,
			O,O,O,O,O,O,
			O,O,O,O,O,O,
			O,O,O,O,O,O,
			O,O,O,O,O,O,
			O,O,M,M,O,O,
			
			O,M,M,M,M,O,
			M,M,O,O,M,M,
			M,O,O,O,O,M,
			M,O,O,O,O,M,
			M,M,O,O,M,M,
			O,M,M,M,M,O
		}
	);
	
	ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#7F7F7FFF")));

	return new Drawable3DCreateConfig_VoxelGrid(
		"_Turret_",//id
		new std::string("_GroundBase_"),//parentId

		new Vector3(0.0f, 0.0f, 0.1f),// pos
		nullptr,//rot
		nullptr,// scale

		0.1f/*voxelSize*/,
		// The z pos is manually set so the entity sits 0.5f under the ground.
		new Vector3(-6.0f / 2.0f + 0.5f, -6.0f / 2.0f + 0.5f, 0.5f),

		voxelGrid, nullptr,
		materialsList, nullptr
	);
}

Drawable3DCreateConfig_VoxelGrid* Template_Turret_CubesCannon_A_V1::buildBarrel() {
	const signed char O = -1;
	const signed char M = 0; // Metal

	Array3D<signed char>* voxelGrid = new Array3D<signed char>(
		4, 4, 2,
		false, true, true,
		new signed char[4 * 4 * 2]{
			O,M,M,O,
			M,O,O,M,
			M,O,O,M,
			O,M,M,O,

			O,O,O,O,
			O,M,M,O,
			O,M,M,O,
			O,O,O,O
		}
	);
	
	ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#BBBBBBFF")));

	return new Drawable3DCreateConfig_VoxelGrid(
		"_Barrel_",//id
		new std::string("_Turret_"),//parentId

		new Vector3(0.0f, 0.0f, 0.25f),// pos
		nullptr,//rot
		nullptr,// scale

		0.1f/*voxelSize*/,
		// The z pos is manually set so the entity sits 0.5f under the ground.
		new Vector3(-4.0f / 2.0f + 0.5f, -4.0f / 2.0f + 0.5f, -1.5f + 0.5f),

		voxelGrid, nullptr,
		materialsList, nullptr
	);
}

Drawable3DCreateConfig_VoxelGrid* Template_Turret_CubesCannon_A_V1::buildProjectile() {
	const signed char O = -1;
	const signed char M = 0; // Metal

	Array3D<signed char>* voxelGrid = new Array3D<signed char>(
		2, 2, 2,
		false, true, true,
		new signed char[2 * 2 * 2]{
			M,M,
			M,M,

			M,M,
			M,M
		}
	);
	
	ArrayList<Visual3DModelMaterial*>* materialsList = new ArrayList<Visual3DModelMaterial*>();
	materialsList->appendDirect(new Visual3DModelMaterial_Color(new Color("#2BE4E7FF")));

	return new Drawable3DCreateConfig_VoxelGrid(
		"_Projectile_",//id
		new std::string("_Barrel_"),//parentId

		new Vector3(0.0f, 0.0f, 0.05f),// pos
		nullptr,//rot
		nullptr,// scale

		0.1f/*voxelSize*/,
		// The z pos is manually set so the entity sits 0.5f under the ground.
		new Vector3(-2.0f / 2.0f + 0.5f, -2.0f / 2.0f + 0.5f, -2.0f / 2.0f + 0.5f),

		voxelGrid, nullptr,
		materialsList, nullptr
	);
}
