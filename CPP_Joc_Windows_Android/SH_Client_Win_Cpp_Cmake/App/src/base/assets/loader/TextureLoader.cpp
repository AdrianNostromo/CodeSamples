#include "TextureLoader.h"
#include <vendor/stb/stb_image.h>
#include <base/opengl/util/GLUtil.h>
#include <assert.h>
#include <base/exceptions/AssetMissingOrInvalidException.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/exceptions/FileNotFoundException.h>
#include <base/exceptions/LogicException.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <base/MM.h>

TextureLoader::TextureLoader(
    IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier,
    const std::string& assetsFolderPath,
    std::shared_ptr<LoaderConfig> config)
	:super(handler, identifier, assetsFolderPath),
    config(config)
{
    //void
}

int TextureLoader::tickStatus(int status, bool isStatusInit) {
	switch (status) {
	case StatusCode::Iddle: {
        if (!isStatusInit) {
            //void
        }

        return LocalStatusCode::Loading;
        //break;
    }
	case LocalStatusCode::Loading: {
        if (!isStatusInit) {
            initStatus_loading();

            return StatusCode::Finished_Success;
        }

        //void

        break;
    }
	default: {
        assert(false);

        break;
    }}

	return status;
}

void TextureLoader::initStatus_loading() {
	stbi_set_flip_vertically_on_load(1);

    int width = 0;
    int height = 0;
    int channelsCountOf8BitsPerChannel = 0;

	std::string fullPath = getAssetsFolderPath() + getSubPath();
    base::Buffer fileData;
    std::shared_ptr<base::IFileHandlerBlocking> fh = base::Util_FileSystem::fs->getFileHandlerBlocking(fullPath, getFsTarget());
    try {
        fileData = fh->readBinarySP();
    }catch(FileNotFoundException& x) {
        throw AssetMissingOrInvalidException();
    }
    if(fileData == nullptr) {
        throw LogicException(LOC);
    }

    int bitsPerPixel_NOT_BYTES = 0;
    // This function is 8 bit.
    
    // If this is false, the texture will receive the unpacked data.
    unsigned char* textureData = nullptr;
    int textureDataBytesCount = 0;

    if (!config->isDataPacked) {
        // Unpacked texture data.
        textureData = stbi_load_from_memory(
            (unsigned char*)fileData.getBlock(), fileData.getBytesCount(),
            &width, &height, &channelsCountOf8BitsPerChannel, 4/*desired_channels*/
        );
        if (textureData == nullptr || channelsCountOf8BitsPerChannel != 4) {
            throw LogicException(LOC);
        }

        bitsPerPixel_NOT_BYTES = channelsCountOf8BitsPerChannel/*channelsCount*/ * 8/*bits_per_channel*/;

        if ((bitsPerPixel_NOT_BYTES % 8) != 0) {
            // The texture bitsPerPixel_NOT_BYTES is not a multiple of 8. Conversion is not valid.
            /// This function should be used only on multiple of 8 bitsPerPixel_NOT_BYTES.
            throw LogicException(LOC);
        }
        textureDataBytesCount = width * height * (bitsPerPixel_NOT_BYTES / 8);
    } else {
        // Packed texture data.
        int r = stbi_info_from_memory(
            (unsigned char*)fileData.getBlock(), fileData.getBytesCount(),
            &width, &height, &channelsCountOf8BitsPerChannel
        );
        if (channelsCountOf8BitsPerChannel != 4) {
            throw LogicException(LOC);
        }

        textureDataBytesCount = fileData.getBytesCount();
        textureData = (unsigned char*)fileData.extractBufferMemory();

        // This should always be correct for this app.
        bitsPerPixel_NOT_BYTES = channelsCountOf8BitsPerChannel/*channelsCount*/ * 8/*bits_per_channel*/;
    }

    // Save
	data = new Texture(
        base::Buffer{ textureData/*block*/, textureDataBytesCount/*bytesCount*/, true/*isBufferOwner*/, false/*isBlockMemTracked*/ }, config->isDataPacked,
		width, height, channelsCountOf8BitsPerChannel, bitsPerPixel_NOT_BYTES,
        1/*mipMapLevels*/, IGL::RGBA8/*sizedInternalFormat*/,
		config->textureMinFilter, config->textureMagFilter,
        config->textureWrapS, config->textureWrapT,
        config->globalTextures
	);
    data->reservedCreate();
}

void* TextureLoader::extractData() {
	Texture* t = data;

	data = nullptr;

	return t;
}

void TextureLoader::disposeMain() {
	//void

	super::disposeMain();
}

TextureLoader::~TextureLoader() {
	if (localBuffer) {
		free(localBuffer);

		localBuffer = nullptr;
	}
	if (data) {
		data->reservedDisposeMain();

		delete data;

		data = nullptr;
	}
}
