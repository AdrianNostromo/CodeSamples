#pragma once

#include <base/gh.h>
#include "IAssetsManager.h"
#include "util/AssetType.h"

class IApp;
namespace graphics {
	class TextureArray;
};

class AM_Base : virtual public IAssetsManager {pub dCtor(AM_Base);
	prot static std::string assetsFolderPath;
	prot static std::string cacheFolderPath;

	prot IApp* app;

	prot ArrayList<graphics::TextureArray*>* globalTextures = nullptr;

	pub explicit AM_Base(IApp* app);
	pub void reservedCreate();
	prot virtual void createMain();

	pub virtual void initGlobalTextures(ArrayList<graphics::TextureArray*>* globalTextures);

	pub void reservedDisposeMain();

	prot virtual void disposeMain();
	pub virtual ~AM_Base() = default;
};
