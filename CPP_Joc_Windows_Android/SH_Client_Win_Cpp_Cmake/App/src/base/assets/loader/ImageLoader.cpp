#include "ImageLoader.h"
#include <vendor/stb/stb_image.h>
#include <base/opengl/util/GLUtil.h>
#include <assert.h>
#include <base/exceptions/LogicException.h>
#include <base/MM.h>

ImageLoader::ImageLoader(IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath, std::shared_ptr<LoaderConfig> config)
	:super(handler, identifier, assetsFolderPath)
{
    if(config != nullptr) {
        this->config = config;
    }else {
        this->config = std::make_shared<LoaderConfig>();
    }
}

int ImageLoader::tickStatus(int status, bool isStatusInit) {
	switch (status) {
	case LocalStatusCode::Iddle: {
        if (!isStatusInit) {
            //void
        }

        return LocalStatusCode::Loading;
        //break;
    }
	case LocalStatusCode::Loading: {
        if (!isStatusInit) {
            initStatus_loading();

            return LocalStatusCode::Finished_Success;
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

void ImageLoader::initStatus_loading() {
    stbi_set_flip_vertically_on_load(1);

    int width = 0;
    int height = 0;
    int channelsCountOf8BitsPerChannel = 0;

    std::string fullPath = getAssetsFolderPath() + getSubPath();
    unsigned char* buff = stbi_load(fullPath.c_str(), &width, &height, &channelsCountOf8BitsPerChannel, 4);
    if(buff == nullptr || channelsCountOf8BitsPerChannel != 4) {
        throw LogicException(LOC);
    }

    int bitsPerPixel_NOT_BYTES = channelsCountOf8BitsPerChannel/*channelsCount*/ * 8/*bits_per_channel*/;

    data = new base::Image(
        new base::Buffer(buff, width * height * channelsCountOf8BitsPerChannel, true/*isBufferOwner*/, false/*isBlockMemTracked*/),
        width, height, bitsPerPixel_NOT_BYTES
    );
}

void* ImageLoader::extractData() {
    base::Image* t = data;

	data = nullptr;

	return t;
}

void ImageLoader::disposeMain() {
	//void

	super::disposeMain();
}

ImageLoader::~ImageLoader() {
	if (data) {
	    delete data;

		data = nullptr;
	}
}
