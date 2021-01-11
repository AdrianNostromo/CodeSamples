#include "GeneratedSoundLoader.h"
#include <base/assets/util/AssetType.h>
#include <base/assets/loader/TextureLoader.h>
#include <base/exceptions/FileNotFoundException.h>
#include <base/exceptions/AssetMissingOrInvalidException.h>
#include <base/util/StringUtil.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/math/Math.h>
#include <base/opengl/util/GLUtil.h>
#include <cstring>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/MM.h>

GeneratedSoundLoader::GeneratedSoundLoader(
    IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier,
    const std::string& assetsFolderPath,
    base::audio::IAudio* audio,
    std::shared_ptr<LoaderConfig> config, std::string cacheFolderPath)
	:super(handler, identifier, assetsFolderPath),
	 audio(audio),
     cacheFolderPath(cacheFolderPath)
{
    if(config != nullptr) {
        this->config = config;
    }else {
        this->config = std::make_shared<LoaderConfig>();
    }
}

int GeneratedSoundLoader::tickStatus(int status, bool isStatusInit) {
	switch (status) {
	case LocalStatusCode::Iddle: {
        if (!isStatusInit) {
            //void
        }

        return LocalStatusCode::Loading_PreDeps;
        //break;
    }
	case LocalStatusCode::Loading_PreDeps: {
		if (!isStatusInit) {
            initStatus_loading_preDependencies();

            return LocalStatusCode::Finished_Success;
		}

        throw LogicException(LOC);

		//break;
    }
	default: {
		return -1;
		//break;
    }}

	//return status;
}

static inline float sin01(float alpha) {
	//printf("alpha: %f\n", alpha);
	return 0.5f * sin(alpha) + 0.5f;
}

void GeneratedSoundLoader::initStatus_loading_preDependencies() {
	int rate = audio->getRate();
	
	base::audio::CfgSound* cfgSound = base::audio::CfgSound::fromId(getSubPath());
	if (cfgSound == nullptr) {
		throw LogicException(LOC);
	}

	std::shared_ptr<base::audio::DtAudioData> soundData = cfgSound->rasterize(rate);
	if (soundData == nullptr) {
		throw LogicException(LOC);
	}

	data = new base::audio::Sound(
		audio,
		soundData
	);

	if (data == nullptr) {
		throw LogicException(LOC);
	}

    cleanup();
}

void* GeneratedSoundLoader::extractData() {
    base::audio::Sound* t = data;

    data = nullptr;

    return t;
}

void GeneratedSoundLoader::cleanup() {
	//void
}

void GeneratedSoundLoader::disposeMain() {
    cleanup();

    super::disposeMain();
}

GeneratedSoundLoader::~GeneratedSoundLoader() {
    if (data) {
        delete data;

        data = nullptr;
    }
}
