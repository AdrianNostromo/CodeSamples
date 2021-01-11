#pragma once

#include <memory>
#include <base/assets/util/Asset.h>

class BasicAssetLoader;

namespace base {
class LoadingAssetWrapper {
public:
    class Status {
    public:
        static const int Loading = 1;
        static const int Finished_Success = 2;
        static const int Failed_AssetMissingOrInvalid = 3;
    };
public:
    std::shared_ptr<base::Asset> asset;

    int status = Status::Loading;

    BasicAssetLoader* loader = nullptr;

public:
    LoadingAssetWrapper(LoadingAssetWrapper const&) = delete;
    LoadingAssetWrapper(LoadingAssetWrapper &&) = default;
    LoadingAssetWrapper& operator=(LoadingAssetWrapper const&) = delete;
    LoadingAssetWrapper& operator=(LoadingAssetWrapper &&) = default;

    explicit LoadingAssetWrapper(std::shared_ptr<base::Asset> asset);

    virtual ~LoadingAssetWrapper();
};
};
