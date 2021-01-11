#pragma once

#include <base/gh.h>
#include "BasicAssetLoader.h"
#include <string>
#include <graphics/visual/Texture.h>
#include <base/assets/loader/util/IAssetLoaderConfig.h>

namespace graphics {
	class TextureArray;
};

class TextureLoader : public BasicAssetLoader {priv typedef BasicAssetLoader super;pub dCtor(TextureLoader);
	pub class LoaderConfig : virtual public IAssetLoaderConfig {
		pub Texture::TextureFilterMin const* textureMinFilter;
		pub Texture::TextureFilterMag const* textureMagFilter;
		pub Texture::TextureWrap const* textureWrapS;
		pub Texture::TextureWrap const* textureWrapT;

		pub ArrayList<graphics::TextureArray*>* globalTextures;

		pub bool isDataPacked;
		
		pub explicit LoaderConfig(
		    Texture::TextureFilterMin const* _textureMinFilter, Texture::TextureFilterMag const* _textureMagFilter,
		    Texture::TextureWrap const* _textureWrapS, Texture::TextureWrap const* _textureWrapT,
			ArrayList<graphics::TextureArray*>* globalTextures, bool isDataPacked)
			: textureMinFilter(_textureMinFilter), textureMagFilter(_textureMagFilter),
			textureWrapS(_textureWrapS), textureWrapT(_textureWrapT),
			globalTextures(globalTextures), isDataPacked(isDataPacked)
		{
			//void
		}
	};

	pub class LocalStatusCode : public StatusCode {priv typedef StatusCode super;
        pub static const int Loading = super::LAST_USED_INDEX + 1;

        prot static const int LAST_USED_INDEX = Loading;

    };

    priv std::shared_ptr<LoaderConfig> config;

	// These are here to make sure cleanup occurs on errors.
	priv unsigned char* localBuffer = nullptr;

	priv Texture* data = nullptr;

	pub explicit TextureLoader(
        IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath,
        std::shared_ptr<LoaderConfig> config);

	pub virtual int tickStatus(int status, bool isStatusInit) override;
	pub virtual void initStatus_loading();
	pub virtual void* extractData() override;

	prot void disposeMain() override;
	pub ~TextureLoader() override;
};
