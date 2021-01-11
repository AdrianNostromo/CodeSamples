#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include "ITool.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/config/ToolConfigBase.h>
#include <base/memory/ISelfSharedPointerContainer.h>

class IAssetsManager;
namespace worldGame3D {
	class IGameWorldGameParts;
};

namespace rpg3D {
class T_Base : public base::Object, virtual public ITool, virtual public mem::ISelfSharedPointerContainer {priv typedef base::Object super;pub dCtor(T_Base);
	priv static int InstanceId_Counter;
	// This is used for tool extra activation events to make sure they are going to the requester tool (in case the u.i. processing is not exact with any tool switching that may occur).
	pub const int instanceId;

	pub ToolConfigBase* config;

	priv bool isCreated = false;

	// Note. This is initialised automatically by the sp.
	// The tools are inside a sp so they will be implemented in the defragmenting memory manager when that becomes available.
	/// Tools still need to be disposed the old way by calling the dispsoe function. Switch to using sp and make the sp call reservedDispose if the object extends IDisposable.
	pub sp<ITool> selfWSP{true/*isWeakPointer*/};

	prot worldGame3D::IGameWorldGameParts* gwGamePartsOptional = nullptr;

    pub explicit T_Base(ToolConfigBase* config);
	pub void reservedCreate(IAssetsManager* assetsManager);
	prot virtual void create(IAssetsManager* assetsManager);

	pub bool getIsCreated();

	pub sp<ITool>& getSelfWSPRef() { return selfWSP; }
	pub sp<ITool> getSelfSP() final;

	pub int getInstanceId() final;

	pub ToolConfigBase* getConfig() final;

	pub bool hasTags(std::string& singleTag);
	pub bool hasTags(ArrayList<std::string>& testTags);
	pub bool hasTags(Array1D<std::string>& testTags);

	pub template <typename T>
	T getConfigAs();

	pub virtual void connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts);
	pub virtual void disconnectFromGamePartsPre();
	prot virtual void disconnectedFromGameParts();

	pub virtual void onGameLoop_localInputPost(float deltaS, int deltaMS, float gameTimeS);
	pub virtual void onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS);

	prot void disposePost() override;
	pub ~T_Base() override;
};

template <typename T>
T T_Base::getConfigAs() {
	T cConfig = dynamic_cast<T>(config);
	if (!cConfig) {
		throw LogicException(LOC);
	}

	return cConfig;
}

};
