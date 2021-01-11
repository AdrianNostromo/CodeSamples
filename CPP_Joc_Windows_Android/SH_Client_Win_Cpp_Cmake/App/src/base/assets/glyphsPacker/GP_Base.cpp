#include "GP_Base.h"
#include <base/exceptions/LogicException.h>

using namespace base;

GP_Base::GP_Base(std::string& cacheId, IAssetsManager* assetsManager, ArrayList<graphics::TextureArray*>* globalTextures, Asset* selfAsset)
	: super(),
	cacheId(cacheId), assetsManager(assetsManager), globalTextures(globalTextures), selfAsset(selfAsset)
{
	//void
}

GP_Base::~GP_Base() {
	//void
}
