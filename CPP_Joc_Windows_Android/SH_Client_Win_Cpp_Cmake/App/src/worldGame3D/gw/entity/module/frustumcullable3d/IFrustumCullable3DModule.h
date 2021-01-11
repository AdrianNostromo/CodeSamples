#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector3.h>

class IFrustumCullable3DHandler;
class FrustumCullable3DTemplate;

class IFrustumCullable3DModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

    pub virtual FrustumCullable3DTemplate* getT() = 0;
    pub virtual FrustumCullable3DTemplate* getTemplate() = 0;

    pub virtual void setFrustumCulling3DHandler(IFrustumCullable3DHandler* frustumCulling3DHandler) = 0;
    pub virtual bool getIsVisible() = 0;
    pub virtual void setIsVisible(bool isVisible) = 0;

    pub ~IFrustumCullable3DModule() override = default;
};
