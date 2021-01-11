#include <rpg3D/Namespace.h>
#include <base/statics/StaticsInit.h>
#include "ProjectileConfigsList.h"
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/config/entity/projectile/T_Projectile_Arrow_Basic_A_V1.h>
#include <rpg3D/gw/config/entity/projectile/T_Projectile_Arrow_Basic_Grid06Range3Tiles_A_V1.h>
#include <rpg3D/gw/config/entity/projectile/T_Projectile_Bolt_Basic_A_V1.h>
#include <rpg3D/gw/config/entity/projectile/T_Projectile_Bullet_Basic_A_V1.h>
#include <rpg3D/gw/config/entity/projectile/T_Projectile_LaserSegment_Basic_A_V1.h>

using namespace rpg3D;

bool ProjectileConfigsList::IsStaticsInitHooked = base::StaticsInit::AddCbGeneral<bool>(1, ProjectileConfigsList::InitStatics);

rpg3D::ProjectileConfig* ProjectileConfigsList::Arrow_Basic_A_V1 = nullptr;
rpg3D::ProjectileConfig* ProjectileConfigsList::Arrow_Basic_Grid06Range3Tiles_A_V1 = nullptr;
rpg3D::ProjectileConfig* ProjectileConfigsList::Bullet_Basic_A_V1 = nullptr;
rpg3D::ProjectileConfig* ProjectileConfigsList::Bolt_Basic_A_V1 = nullptr;
rpg3D::ProjectileConfig* ProjectileConfigsList::LaserSegment_Basic_A_V1 = nullptr;

ProjectileConfig* ProjectileConfigsList::newEntry(std::string id) {
	return ProjectileConfig::newEntry(Namespace::NAMESPACE, id);
}

void ProjectileConfigsList::InitStatics() {
	Arrow_Basic_A_V1 = newEntry("Arrow_Basic_A_V1");
	Arrow_Basic_A_V1->init(rpg3D::Namespace::NAMESPACE, rpg3D::T_Projectile_Arrow_Basic_A_V1::getOrCreateTemplate()->id);
	Arrow_Basic_A_V1->effectPayloadsList.appendDirect(std::make_shared<rpg3D::EffectPayload>(rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, 1.0f));

	Arrow_Basic_Grid06Range3Tiles_A_V1 = newEntry("Arrow_Basic_Grid06Range3Tiles_A_V1 ");
	Arrow_Basic_Grid06Range3Tiles_A_V1->init(rpg3D::Namespace::NAMESPACE, rpg3D::T_Projectile_Arrow_Basic_Grid06Range3Tiles_A_V1::getOrCreateTemplate()->id);
	Arrow_Basic_Grid06Range3Tiles_A_V1->effectPayloadsList.appendDirect(std::make_shared<rpg3D::EffectPayload>(rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, 1.0f));

	Bullet_Basic_A_V1 = newEntry("Bullet_Basic_A_V1");
	Bullet_Basic_A_V1->init(rpg3D::Namespace::NAMESPACE, rpg3D::T_Projectile_Bullet_Basic_A_V1::getOrCreateTemplate()->id);
	Bullet_Basic_A_V1->effectPayloadsList.appendDirect(std::make_shared<rpg3D::EffectPayload>(rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, 1.0f));

	Bolt_Basic_A_V1 = newEntry("Bolt_Basic_A_V1");
	Bolt_Basic_A_V1->init(rpg3D::Namespace::NAMESPACE, rpg3D::T_Projectile_Bolt_Basic_A_V1::getOrCreateTemplate()->id);
	Bolt_Basic_A_V1->effectPayloadsList.appendDirect(std::make_shared<rpg3D::EffectPayload>(rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, 1.0f));

	LaserSegment_Basic_A_V1 = newEntry("LaserSegment_Basic_A_V1");
	LaserSegment_Basic_A_V1->init(rpg3D::Namespace::NAMESPACE, rpg3D::T_Projectile_LaserSegment_Basic_A_V1::getOrCreateTemplate()->id);
	LaserSegment_Basic_A_V1->effectPayloadsList.appendDirect(std::make_shared<rpg3D::EffectPayload>(rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, 1.0f));
}
