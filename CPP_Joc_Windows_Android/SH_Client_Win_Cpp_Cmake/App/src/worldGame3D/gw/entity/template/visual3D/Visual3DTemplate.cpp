#include "Visual3DTemplate.h"
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <graphics/visual3d/drawable/IDrawableNode3D.h>
#include <base/map/Map1D.h>
#include <base/visual3d/Visual3DUtil.h>
#include <graphics/visual3d/drawable/IContainer3D.h>
#include <worldGame3D/gw/IEntityModuleCreator.h>
#include <base/exceptions/LogicException.h>

Visual3DTemplate::Visual3DTemplate(
    Visual3DCreateConfig* _createConfig,
    bool _requiresHalfGridOffsetX, bool _requiresHalfGridOffsetY)
    : super("visual3D", worldGame3D::IVisual3DModule::TYPE),
    createConfig(_createConfig), requiresHalfGridOffsetX(_requiresHalfGridOffsetX), requiresHalfGridOffsetY(_requiresHalfGridOffsetY)
{
    //void
}

IEntityModule* Visual3DTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    Visual3DTemplate* t = dynamic_cast<Visual3DTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    ArrayList<IDrawableNode3D*>* drawablesList = new ArrayList<IDrawableNode3D*>();
    Map1D<std::string, IDrawableNode3D*>* drawablesMap = new Map1D<std::string, IDrawableNode3D*>();
    IDrawableNode3D* rootDrawable = nullptr;

    rootDrawable = Visual3DUtil::createConfiguredVisual3D(
        createConfig,
        drawablesList = new ArrayList<IDrawableNode3D*>(), drawablesMap = new Map1D<std::string, IDrawableNode3D*>(),
        nullptr/*visualParent*/, extraData,
        false/*doReservedCreateVisuals*/
    );
    if(rootDrawable == nullptr) {
        throw LogicException(LOC);
    }

    worldGame3D::Visual3DModule* mod = new worldGame3D::Visual3DModule(
        entity,
        t,
        drawablesList,
        drawablesMap,
        rootDrawable
    );

    mod->setVisualsParent(moduleCreator->getLayerGameWorld3D());

    return mod;
}

Visual3DTemplate::~Visual3DTemplate() {
    //void
}
