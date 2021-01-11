#include "TRIP_ProjectilePreviewVisual.h"
#include <base/visual3d/Visual3DUtil.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <worldGame3D/gw/entity/module/visual3D/IVisual3DModule.h>
#include <base/visual3d/IManagedDynamicVisual.h>
#include <rpg3D/gw/projectile/ProjectileConfig.h>

using namespace rpg3D;

TRIP_ProjectilePreviewVisual::TRIP_ProjectilePreviewVisual(ToolConfigRangeInertialProjectile* config, std::shared_ptr<ExtraData> extraData)
    : super(config, extraData)
{
    //void
}

void TRIP_ProjectilePreviewVisual::requireVisualsUpdate() {
    if(!isVisualsDirty) {
        isVisualsDirty = true;

        entity->getHandler()->addDrawableModuleToVisualUpdate(this);
    }
}

void TRIP_ProjectilePreviewVisual::requireManagedVisualUpdate(IManagedDynamicVisual* targetManagedVisual) {
    super::requireManagedVisualUpdate(targetManagedVisual);

    requireVisualsUpdate();
}

void TRIP_ProjectilePreviewVisual::onProjectileConfigChanged() {
    super::onProjectileConfigChanged();

    // Clear existing visuals.
	disposeDrawables();

    checkProjectileVisualUpdate();
}

void TRIP_ProjectilePreviewVisual::onEntityChangePre() {
    super::onEntityChangePre();

    //void
}

void TRIP_ProjectilePreviewVisual::onEntityChanged() {
    super::onEntityChanged();

    if (getLocalConfig()->projectilePreviewParentId != nullptr) {
        projectilePreviewParent = getToolDrawable<IContainer3D*>(
            *getLocalConfig()->projectilePreviewParentId, 
            false/*mustExist*/
        );
    }

    checkProjectileVisualUpdate();
}

void TRIP_ProjectilePreviewVisual::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
    super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

    // Show or hide the projectile preview visual. Keep the model in memory, just remove it from the display list.
    if(!isSelectedAndGWCapabilitiesExist) {
        checkProjectileVisualUpdate();
    }else {
        checkProjectileVisualUpdate();
    }
}

void TRIP_ProjectilePreviewVisual::onCooldownStateChanged() {
    super::onCooldownStateChanged();

    checkProjectileVisualUpdate();
}

void TRIP_ProjectilePreviewVisual::checkProjectileVisualUpdate() {
    //States:
    /// disposed
    /// exists_hidden
    /// exists_visible

    if (entity != nullptr && getLocalConfig()->projectilePreviewParentId != nullptr/*preview_enabled*/ && hasProjectileConfig()) {
        // Tool is on entity and a projectile config exists. Create drawables when the tool is on a entity.
        if (rootDrawable == nullptr) {
            EntityTemplate* projectileTemplate = EntityTemplate::fromId(
                peekProjectileConfig()->projectileTemplateNSpace,
                peekProjectileConfig()->projectileTemplateId
            );
            Visual3DTemplate* visual3DTemplate = projectileTemplate->getModuleTemplateAs<Visual3DTemplate*>(worldGame3D::IVisual3DModule::TYPE);

            rootDrawable = Visual3DUtil::createConfiguredVisual3D(
                visual3DTemplate->createConfig,
                drawablesList = new ArrayList<IDrawableNode3D*>(), drawablesMap = new Map1D<std::string, IDrawableNode3D*>(),
                nullptr/*visualParent*/, nullptr,
                true
            );
        }

        if (getIsToolSelected() && canStartTriggering()) {
            if (rootDrawable->getParent() == nullptr) {
                if (projectilePreviewParent == nullptr) {
                    throw LogicException(LOC);
                }

                projectilePreviewParent->addChild(rootDrawable);
            }
        }else {
            if (rootDrawable->getParent() != nullptr) {
                rootDrawable->getParent()->removeChild(rootDrawable);
            }
        }
    } else {
        disposeDrawables();
    }
}

void TRIP_ProjectilePreviewVisual::updateVisual() {
    super::updateVisual();

    isVisualsDirty = false;

    if (drawablesList != nullptr) {
        for (int i = drawablesList->size() - 1; i >= 0; i--) {
            IDrawableNode3D* drawable = *drawablesList->getPointer(i);
            if (drawable != nullptr) {
                IManagedDynamicVisual* lDrawable = dynamic_cast<IManagedDynamicVisual*>(drawable);
                if (lDrawable) {
                    lDrawable->updateVisual();
                }
            }
        }
    }
}

void TRIP_ProjectilePreviewVisual::disposeDrawables() {
	if(rootDrawable != nullptr) {
		rootDrawable = nullptr;
	}

	if(drawablesMap != nullptr) {
		delete drawablesMap;
		drawablesMap = nullptr;
	}

	if(drawablesList != nullptr) {
		Visual3DUtil::disposeDrawables(drawablesList);

		delete drawablesList;
		drawablesList = nullptr;
	}
}

void TRIP_ProjectilePreviewVisual::disposeMain() {
    disposeDrawables();

    super::disposeMain();
}

TRIP_ProjectilePreviewVisual::~TRIP_ProjectilePreviewVisual() {
    //void
}
