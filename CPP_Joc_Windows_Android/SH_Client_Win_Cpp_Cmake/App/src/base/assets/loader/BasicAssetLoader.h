#pragma once

#include <base/gh.h>
#include <string>
#include <base/list/ArrayList.h>
#include <base/assets/loader/util/AssetLoaderDependency.h>
#include "./../util/AssetIdentifier.h"
#include <base/assets/util/Asset.h>
#include <chrono>

class IAssetLoaderHandler;

class BasicAssetLoader {pub dCtor(BasicAssetLoader);
    pub class StatusCode {
        pub static const int Iddle = 1;

        pub static const int Finished_Success = 2;
        pub static const int Finished_Failed = 3;

        prot static const int LAST_USED_INDEX = Finished_Failed;

    };

    pub class ErrorCode {
        pub static const int None = 0;
        pub static const int AssetMissingOrInvalid = 1;
        pub static const int LogicError = 2;
	};

    priv int status = StatusCode::Iddle;
    priv bool isStatusInit = false;
    priv int errorCode = ErrorCode::None;

    priv IAssetLoaderHandler* handler;

    priv std::shared_ptr<AssetIdentifier> identifier;
    priv int fsTarget;

    priv std::string assetsFolderPath;

	// This is nullptr for controller loaders.
    priv std::shared_ptr<base::LoadingAssetWrapper> loadingAsset = nullptr;
    // These must be created when the data is extracted.
    priv ArrayList<std::shared_ptr<base::Asset>> newSubDepsList{};

    priv ArrayList<std::shared_ptr<AssetLoaderDependency>> loaderDependenciesList{};
    priv bool isWaitingForDependencies = false;

    priv std::chrono::time_point<std::chrono::system_clock> creationTimestamp;

    pub explicit BasicAssetLoader(IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath);
    pub void initData(std::shared_ptr<base::LoadingAssetWrapper> loadingAsset, int fsTarget);
    pub void reservedCreate();

    pub std::shared_ptr<base::LoadingAssetWrapper> getLoadingAsset();
    pub ArrayList<std::shared_ptr<base::Asset>>* getNewSubDepsList();

    pub int getFsTarget();

    pub int loadTick();
    pub virtual int tickStatus(int status, bool isStatusInit) = 0;
    pub std::shared_ptr<AssetIdentifier> getIdentifier() { return identifier; }
    pub AssetType const* getType() { return identifier->type; }
    pub std::string& getAssetsFolderPath() { return assetsFolderPath; }
    pub std::string& getSubPath() { return identifier->id; }
    pub int getStatus() { return status; }
    pub int getErrorCode() { return errorCode; }
    pub IAssetLoaderHandler* getHandler() { return handler; }
    pub void setErrorCode(int errorCode);
    pub virtual void* extractData() = 0;

    pub bool getIsWaitingForDependencies();
    pub void clearIsWaitingForDependencies();
    pub ArrayList<std::shared_ptr<AssetLoaderDependency>>* peekLoaderDependenciesList();

    prot virtual void createMain();

    prot bool isDependenciesLoaded();

    prot std::shared_ptr<AssetLoaderDependency> getDependency(AssetType const* type, std::string& id);
    prot void pushDependency(std::shared_ptr<AssetLoaderDependency> dep);
    prot void removeDependency(std::shared_ptr<AssetLoaderDependency> dep);
    // This is called on failed loaders.
    prot void cleanupDependencies();

    prot virtual void disposeMain();

    pub void reservedDisposeMain();
    pub virtual ~BasicAssetLoader();
};
