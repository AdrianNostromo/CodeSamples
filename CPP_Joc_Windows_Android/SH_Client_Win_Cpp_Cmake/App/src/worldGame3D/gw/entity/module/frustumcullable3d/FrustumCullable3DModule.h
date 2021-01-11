#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/frustumCUllable3D/FrustumCullable3DTemplate.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IFrustumCullable3DModule.h"
#include <worldGame3D/gw/entity/module/frustumcullable3d/util/IFrustumCullable3DHandler.h>

class IWorldEntity;
namespace worldGame3D { class IVisual3DModule; }

class FrustumCullable3DModule : public EntityModule, public virtual IFrustumCullable3DModule {priv typedef EntityModule super;pub dCtor(FrustumCullable3DModule);
    pub FrustumCullable3DTemplate* t;

    prot IFrustumCullable3DHandler* frustumCulling3DHandler = nullptr;

    prot worldGame3D::IVisual3DModule* visual3D = nullptr;

    priv bool isVisible = true;

    pub explicit FrustumCullable3DModule(
        IWorldEntity* entity,
        FrustumCullable3DTemplate* t
    );
    prot void createBMain() override;

    pub FrustumCullable3DTemplate* getT() final;
    pub FrustumCullable3DTemplate* getTemplate() final;

    priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

    pub void setFrustumCulling3DHandler(IFrustumCullable3DHandler* frustumCulling3DHandler) final;
    pub bool getIsVisible() final;
    pub void setIsVisible(bool isVisible) final;

    pub ~FrustumCullable3DModule() override;
};
