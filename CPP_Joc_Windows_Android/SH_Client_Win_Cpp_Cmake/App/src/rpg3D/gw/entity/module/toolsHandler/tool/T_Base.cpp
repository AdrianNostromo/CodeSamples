#include "T_Base.h"
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

int T_Base::InstanceId_Counter = 0;

T_Base::T_Base(ToolConfigBase* config)
	: super(),
    instanceId(++InstanceId_Counter),
	config(config)
{
	//void
}

void T_Base::reservedCreate(IAssetsManager* assetsManager) {
    if (isCreated) {
        // Already created.
        throw LogicException(LOC);
    }

    isCreated = true;

    create(assetsManager);
}

void T_Base::create(IAssetsManager* assetsManager) {
    if (selfWSP == nullptr) {
        // Must manually init selftSP before reservedCreate is called.
        throw LogicException(LOC);
    }
}

bool T_Base::getIsCreated() {
    return isCreated;
}

sp<ITool> T_Base::getSelfSP() {
    return selfWSP;
}

int T_Base::getInstanceId() {
    return instanceId;
}

ToolConfigBase* T_Base::getConfig() {
	return config;
}

bool T_Base::hasTags(std::string& singleTag) {
    if (config->tagsList == nullptr || config->tagsList->indexOfReference(singleTag) < 0) {
        return false;
    }

    return true;
}

bool T_Base::hasTags(ArrayList<std::string>& testTags) {
    for (int i = 0; i < testTags.size(); i++) {
        if (config->tagsList->indexOfReference(testTags.getReference(i)) < 0) {
            return false;
        }
    }

    return true;
}

bool T_Base::hasTags(Array1D<std::string>& testTags) {
    for (int i = 0; i < testTags.getLength(); i++) {
        if (config->tagsList->indexOfReference(testTags.getReference(i)) < 0) {
            return false;
        }
    }

    return true;
}

void T_Base::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
    gwGamePartsOptional = gwGameParts;
}

void T_Base::disconnectFromGamePartsPre() {
    gwGamePartsOptional = nullptr;

    disconnectedFromGameParts();
}

void T_Base::disconnectedFromGameParts() {
    //void
}

void T_Base::onGameLoop_localInputPost(float deltaS, int deltaMS, float gameTimeS) {
    //void
}

void T_Base::onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) {
    //void
}

void T_Base::disposePost() {
    // selfSPPointer is cleared by the sp only.
    //selfSPPointer = nullptr;

    super::disposePost();
}

T_Base::~T_Base() {
	//void
}
