#include "BasicAssetLoader.h"
#include <base/exceptions/LogicException.h>
#include <base/exceptions/AssetMissingOrInvalidException.h>
#include <base/assets/loader/util/IAssetLoaderHandler.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/config/AppConfig.h>
BasicAssetLoader::BasicAssetLoader(IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath)
    : handler(handler), identifier(identifier), assetsFolderPath(assetsFolderPath)
{
	//void
}

void BasicAssetLoader::initData(std::shared_ptr<base::LoadingAssetWrapper> loadingAsset, int fsTarget) {
    this->loadingAsset = loadingAsset;
    this->fsTarget = fsTarget;
}

void BasicAssetLoader::reservedCreate() {
    createMain();
}

void BasicAssetLoader::createMain() {
    if(this->loadingAsset == nullptr) {
        // Class init error.
        throw LogicException(LOC);
    }

    if (AppConfig::IS_DEBUG && AppConfig::IS_DEBUG_AssetsLoadDurations) {
        creationTimestamp = std::chrono::system_clock::now();
    }
}

std::shared_ptr<base::LoadingAssetWrapper> BasicAssetLoader::getLoadingAsset() {
    return loadingAsset;
}

ArrayList<std::shared_ptr<base::Asset>>* BasicAssetLoader::getNewSubDepsList() {
    return &newSubDepsList;
}

int BasicAssetLoader::getFsTarget() {
    return fsTarget;
}

int BasicAssetLoader::loadTick() {
    int newStatus;
    try {
        newStatus = tickStatus(status, isStatusInit);
    }catch(AssetMissingOrInvalidException& x) {
        setErrorCode(ErrorCode::AssetMissingOrInvalid);
        newStatus = StatusCode::Finished_Failed;
    }catch(LogicException& x) {
        setErrorCode(ErrorCode::LogicError);
        newStatus = StatusCode::Finished_Failed;
    }
	if (newStatus < 0) {
        setErrorCode(ErrorCode::LogicError);
        newStatus = StatusCode::Finished_Failed;
    }

	if (newStatus != status) {
		status = newStatus;
		isStatusInit = false;

        if (AppConfig::IS_DEBUG && AppConfig::IS_DEBUG_AssetsLoadDurations) {
            if (status == StatusCode::Finished_Success || status == StatusCode::Finished_Failed) {
                std::chrono::time_point<std::chrono::system_clock> cTimectamp = std::chrono::system_clock::now();

                auto autoAssetLoadDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(cTimectamp - creationTimestamp);
                long assetLoadDurationMs = static_cast<int>(autoAssetLoadDurationMs.count());
                float durationS = assetLoadDurationMs / 1000.0f;

                if (status == StatusCode::Finished_Success) {
                    GlobalAccessLogHandler::devLogHandler->post_debug("<Asset loader> Finished_Success in %f seconds, assetType: %s, id: %s", durationS, identifier->type->name.c_str(), identifier->id.c_str());
                } else if (status == StatusCode::Finished_Failed) {
                    GlobalAccessLogHandler::devLogHandler->post_debug("<Asset loader> Finished_Failed  in %f seconds, assetType: %s, id: %s", durationS, identifier->type->name.c_str(), identifier->id.c_str());
                } else {
                    throw LogicException(LOC);
                }
            }
        }
	} else {
		isStatusInit = true;
	}

	return 0;
}

bool BasicAssetLoader::isDependenciesLoaded() {
    if(!isWaitingForDependencies) {
        return true;
    }

    return false;
}

void BasicAssetLoader::setErrorCode(int errorCode) {
    if(this->errorCode != ErrorCode::None) {
        // Error code already set.
        throw LogicException(LOC);
    }

    this->errorCode = errorCode;
}

bool BasicAssetLoader::getIsWaitingForDependencies() {
    return isWaitingForDependencies;
}

void BasicAssetLoader::clearIsWaitingForDependencies() {
    if(!isWaitingForDependencies) {
        throw LogicException(LOC);
    }

    isWaitingForDependencies = false;
}

ArrayList<std::shared_ptr<AssetLoaderDependency>>* BasicAssetLoader::peekLoaderDependenciesList() {
    return &loaderDependenciesList;
}

std::shared_ptr<AssetLoaderDependency> BasicAssetLoader::getDependency(AssetType const* type, std::string& id) {
    for(int i=0;i<loaderDependenciesList.size();i++) {
        std::shared_ptr<AssetLoaderDependency> dep = loaderDependenciesList.getDirect(i);
        if(dep->assetIdentifier->type == type && dep->assetIdentifier->id == id) {
            return dep;
        }
    }

    return nullptr;
}

void BasicAssetLoader::pushDependency(std::shared_ptr<AssetLoaderDependency> dep) {
    if(loaderDependenciesList.contains(dep)) {
        throw LogicException(LOC);
    }

    isWaitingForDependencies = true;

    loaderDependenciesList.appendDirect(dep);
}

void BasicAssetLoader::removeDependency(std::shared_ptr<AssetLoaderDependency> dep) {
    if(!loaderDependenciesList.remove(dep)) {
        throw LogicException(LOC);
    }

    getHandler()->removeAssetDependency(
        getIdentifier(),
        dep->assetIdentifier
    );
}

void BasicAssetLoader::cleanupDependencies() {
    while(loaderDependenciesList.size() > 0) {
        std::shared_ptr<AssetLoaderDependency> dep = loaderDependenciesList.getDirect(loaderDependenciesList.size() - 1);

        removeDependency(dep);
    }
}

void BasicAssetLoader::reservedDisposeMain() {
    disposeMain();
}

void BasicAssetLoader::disposeMain() {
	//void
}

BasicAssetLoader::~BasicAssetLoader() {
	//void
}
