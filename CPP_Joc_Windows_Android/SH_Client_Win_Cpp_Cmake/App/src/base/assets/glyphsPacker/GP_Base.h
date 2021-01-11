#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include "IGlyphsPacker.h"
#include <string>

class IApp;
class IAssetsManager;
namespace graphics {
    class TextureArray;
};

namespace base {
class GP_Base : public Object, public virtual IGlyphsPacker {priv typedef Object super;pub dCtor(GP_Base);
    prot std::string& cacheId;

    prot IAssetsManager* assetsManager;

    prot ArrayList<graphics::TextureArray*>* globalTextures;

    // This is used to manage dynamic dependencies.
    prot Asset* selfAsset;

    pub explicit GP_Base(std::string& cacheId, IAssetsManager* assetsManager, ArrayList<graphics::TextureArray*>* globalTextures, Asset* selfAsset);

    pub ~GP_Base() override;
};
};
