#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector3.h>

class IReferenceFrameModule;

class IReferenceFrameActorModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual IReferenceFrameModule* getReferenceFrame() = 0;
	pub virtual void setReferenceFrame(IReferenceFrameModule* referenceFrame) = 0;

    pub ~IReferenceFrameActorModule() override = default;
};
