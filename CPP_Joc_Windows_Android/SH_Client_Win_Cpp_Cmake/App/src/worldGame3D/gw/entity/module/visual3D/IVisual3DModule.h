#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>

class IDrawableNode3D;
class IContainer3D;
class Quaternion;
class Matrix4;

namespace worldGame3D {
class IVisual3DModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual IContainer3D* getVisualsParent() = 0;
	pub virtual void setVisualsParent(IContainer3D* visualsParent) = 0;
	
	pub virtual IContainer3D* peekRootDrawableAsContainer() = 0;
	pub virtual Quaternion* peekRootDrawable_rotation() = 0;
	pub virtual Matrix4* peekRootDrawable_globalTransform(bool updateTransformStackIfDirty) = 0;
	pub virtual bool peekRootDrawable_isGlobalTransformInit() = 0;

	pub virtual IDrawableNode3D* getDrawableOptional(std::string id) = 0;
	pub virtual IDrawableNode3D* getDrawableMustExist(std::string id) = 0;

	pub virtual bool getIsVisible() = 0;
	pub virtual void setIsVisibleIncremental(bool visible) = 0;

	template <typename T>
	T getDrawableOptionalAs(std::string id);
	template <typename T>
	T getDrawableMustExistAs(std::string id);

    pub ~IVisual3DModule() override = default;
};

template <typename T>
T IVisual3DModule::getDrawableOptionalAs(std::string id) {
	IDrawableNode3D* v = getDrawableOptional(id);
	if (!v) {
		return nullptr;
	}

	return dynamic_cast<T>(v);
}

template <typename T>
T IVisual3DModule::getDrawableMustExistAs(std::string id) {
	IDrawableNode3D* v = getDrawableMustExist(id);
	if (!v) {
		throw LogicException(LOC);
	}

	return dynamic_cast<T>(v);
}

};
