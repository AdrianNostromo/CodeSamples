#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/ILogicBlock.h>

namespace rpg3D {
class IControllerModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual void* getLogicBlockMustExist(int id) = 0;
	pub virtual void* getLogicBlockOptional(int id) = 0;
	
	pub template <typename T>
	T getLogicBlockOptionalAs(int id);
	pub template <typename T>
	T getLogicBlockMustExistAs(int id);

	pub virtual void registerLogicBlock(ILogicBlock* logicBlock, int id, void* logicBlockInterfaceRawPtr) = 0;

	// lbID == -1; Disable all.
	// lbID >= 0; Enable a single lb and disable all others. The lb must exist or an error will be thrown.
	pub virtual void setSingleActiveGroupLogicBlock(int groupID, int lbID) = 0;

	pub ~IControllerModule() override = default;
};

template <typename T>
T IControllerModule::getLogicBlockOptionalAs(int id) {
	void* lBlock = getLogicBlockOptional(id);
	if (lBlock == nullptr) {
		return nullptr;
	}

	T cLBlock = static_cast<T>(lBlock);
	ILogicBlock* iMod = static_cast<ILogicBlock*>(cLBlock);
	if (iMod->getId() != id) {
		throw LogicException(LOC);
	}

	return cLBlock;
}

template <typename T>
T IControllerModule::getLogicBlockMustExistAs(int id) {
	void* lBlock = getLogicBlockMustExist(id);

	T cLBlock = static_cast<T>(lBlock);
	ILogicBlock* iMod = static_cast<ILogicBlock*>(cLBlock);
	if (iMod->getId() != id) {
		throw LogicException(LOC);
	}

	return cLBlock;
}

};
