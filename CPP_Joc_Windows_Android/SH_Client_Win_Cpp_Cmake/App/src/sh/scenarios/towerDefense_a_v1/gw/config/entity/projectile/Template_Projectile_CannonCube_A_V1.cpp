#include "Template_Projectile_CannonCube_A_V1.h"
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <base/visual3d/util/Drawable3DCreateConfig_Group.h>
#include <rpg3D/gw/entity/template/targetedProjectile/TargetedProjectileTemplate.h>
#include <base/statics/StaticsInit.h>
#include <sh/scenarios/towerDefense_a_v1/Namespace.h>
#include <sh/scenarios/towerDefense_a_v1/gw/config/sounds/SoundConfigsList.h>
#include <rpg3D/gw/entity/template/flying/FlyingTemplate.h>
#include <rpg3D/gw/entity/template/autoDestroyDuration/AutoDestroyDurationTemplate.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>

using namespace towerDefense_a_v1;

std::string Template_Projectile_CannonCube_A_V1::ID = "Projectile_CannonCube_A_V1";

EntityTemplate* Template_Projectile_CannonCube_A_V1::TEMPLATE = base::StaticsInit::AddCbGeneral<EntityTemplate*>(1, []() {
	EntityTemplate* t = TEMPLATE = EntityTemplate::newEntry(Namespace::NAMESPACE, ID);

    t->tagsList.appendReference(rpg3D::EntityTags::PROJECTILE);

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
                        "_contentHolder_",
                        new std::string("_ROOT_"),
                        nullptr,
						nullptr,// rot
                        nullptr
                    ))
					->appendDirect_chain(new Drawable3DCreateConfig_Group(
                        "_damagePoint_",
                        new std::string("_contentHolder_"),
                        new Vector3(0.0f, 0.0f, 0.0f),
                        nullptr,
                        nullptr// scale should not be applied here.
                    ))
                    ->appendDirect_chain(buildBase())
                )
        ),
        false,//requiresHalfGridOffsetX
        false//requiresHalfGridOffsetY
    ));

	t->setModuleTemplate(new worldGame3D::ActionsTemplate(
		//void
	));

	t->setModuleTemplate(new rpg3D::FlyingTemplate(
		5.0f/*flyingSpeedS*/
	));

	t->setModuleTemplate(new rpg3D::AutoDestroyDurationTemplate(
		5.0f/*lifeDistM*/, 5.0f/*lifeDurationS*/
	));

	t->setModuleTemplate(new rpg3D::TargetedProjectileTemplate(
		&SoundConfigsList::CannonProjectileImpact_A_V1->managedPath->path, 0.1f
    ));
});

Drawable3DCreateConfig_VoxelGrid* Template_Projectile_CannonCube_A_V1::buildBase() {
    const signed char O = -1;
	const signed char M = 0; // Metal
	
    Array3D<signed char>* padVoxelGrid = new Array3D<signed char>(
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
        "_VoxelGridGroup_",//id
        new std::string("_contentHolder_"),//parentId

        nullptr,// pos
        nullptr,// rot
        nullptr,// scale

		0.06f/*voxelSize*/,
		// The z pos is manually set so the entity sits 0.5f under the ground.
		new Vector3(-2.0f / 2.0f + 0.5f, -2.0f / 2.0f + 0.5f, -2.0f / 2.0f + 0.5f),

        padVoxelGrid, nullptr,
        materialsList, nullptr
    );
}
