#pragma once

#include <base/gh.h>
#include <base/assets/IAssetsManager.h>

class IAssetLoaderHandler : virtual public IAssetsManager {
    //void

    pub virtual ~IAssetLoaderHandler() = default;
};
