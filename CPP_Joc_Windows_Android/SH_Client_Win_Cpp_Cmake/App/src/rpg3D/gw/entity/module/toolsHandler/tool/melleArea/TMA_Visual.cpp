#include <worldGame3D/gw/entity/WorldEntity.h>
#include "TMA_Visual.h"
#include <base/visual3d/IManagedDynamicVisual.h>
#include <base/visual3d/Visual3DUtil.h>
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <rpg3D/gw/entity/template/toolshandler/util/ToolSlotConfig.h>

using namespace rpg3D;

TMA_Visual::TMA_Visual(ToolConfigMelleArea* config, std::shared_ptr<ExtraData> extraData)
    : super(config, extraData)
{
    //void
}

IWorldEntity* TMA_Visual::getEntity() {
	return super::getEntity();
}

IAppAssets* TMA_Visual::getAppAssets() {
    return entity->getHandler()->getHandler()->getAppAssets();
}

void TMA_Visual::requireVisualsUpdate() {
    if(!isVisualsDirty) {
        isVisualsDirty = true;

        entity->getHandler()->addDrawableModuleToVisualUpdate(this);
    }
}

void TMA_Visual::requireManagedVisualUpdate(IManagedDynamicVisual* targetManagedVisual) {
    requireVisualsUpdate();
}

void TMA_Visual::onEntityChangePre() {
    super::onEntityChangePre();

    // Dispose visuals if exists.
    disposeVisualsIfAny();
}

void TMA_Visual::onEntityChanged() {
    super::onEntityChanged();

    if (entity != nullptr) {
        if (rootDrawable != nullptr) {
            throw LogicException(LOC);
        }

        if (getLocalConfig()->toolVisualCreateConfig != nullptr) {
            rootDrawable = Visual3DUtil::createConfiguredVisual3D(
                getLocalConfig()->toolVisualCreateConfig,
                drawablesList = new ArrayList<IDrawableNode3D*>(), drawablesMap = new Map1D<std::string, IDrawableNode3D*>(),
                nullptr/*visualParent*/, extraData,
                true
            );

            if (getIsToolSelected()) {
                worldGame3D::IVisual3DModule* visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
                IContainer3D* slotVisualHolder = visual3D->getDrawableOptionalAs<IContainer3D*>(peekSelectedToolSlot()->toolHolderVisualId);
                slotVisualHolder->addChild(rootDrawable);
            }
        }
    }
}

void TMA_Visual::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
    super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

    // Show or hide the sword visual. Keep the model in memory, just remove it from the display list.
    if(!isSelectedAndGWCapabilitiesExist) {
        // Hide the visual.

        if (getLocalConfig()->toolVisualCreateConfig != nullptr) {
            if (rootDrawable == nullptr || rootDrawable->getParent() == nullptr) {
                throw LogicException(LOC);
            }

            rootDrawable->getParent()->removeChild(rootDrawable);
        }
    }else {
        // Show the visual.
        if (getLocalConfig()->toolVisualCreateConfig != nullptr) {
            if (rootDrawable == nullptr || rootDrawable->getParent() != nullptr) {
                throw LogicException(LOC);
            }

            worldGame3D::IVisual3DModule* visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
            IContainer3D* slotVisualHolder = visual3D->getDrawableOptionalAs<IContainer3D*>(peekSelectedToolSlot()->toolHolderVisualId);
            slotVisualHolder->addChild(rootDrawable);

            requireVisualsUpdate();
        }
    }
}

IListEntry*& TMA_Visual::getInHandler_VisualUpdateListEntryRef() {
    return inHandler_VisualUpdateListEntryRef;
}

void TMA_Visual::updateVisual() {
    isVisualsDirty = false;

    for(int i=drawablesList->size()-1;i>=0;i--) {
        IDrawableNode3D* drawable = drawablesList->getDirect(i);
        if (drawable != nullptr) {
            IManagedDynamicVisual* lDrawable = dynamic_cast<IManagedDynamicVisual*>(drawable);
            if(lDrawable != nullptr) {
                lDrawable->updateVisual();
            }
        }
    }
}

void TMA_Visual::disposeVisualsIfAny() {
    rootDrawable = nullptr;

    if (drawablesMap != nullptr) {
        delete drawablesMap;
        drawablesMap = nullptr;
    }

    if (drawablesList != nullptr) {
        Visual3DUtil::disposeDrawables(drawablesList);

        delete drawablesList;
        drawablesList = nullptr;
    }
}

void TMA_Visual::disposeMain() {
    // Dispose visuals if exists.
    disposeVisualsIfAny();

    super::disposeMain();
}

TMA_Visual::~TMA_Visual() {
    //void
}
