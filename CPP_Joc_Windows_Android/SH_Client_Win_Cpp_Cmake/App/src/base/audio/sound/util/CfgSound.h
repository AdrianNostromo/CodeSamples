#pragma once

#include <base/map/Map1D.h>
#include <base/audio/sound/source/CfgSoundSource.h>
#include <string>
#include <base/assets/util/ManagedAssetPath.h>

namespace base {
	namespace audio {
		class ISound;
	};
};

namespace base {
namespace audio {
class CfgSound {
public:
	static Map1D<std::string, CfgSound*> entriesMap;

	// nSpace:id
	ManagedAssetPath* managedPath;

	CfgSoundSource* source;

public:
    CfgSound(CfgSound const&) = delete;
    CfgSound(CfgSound &&) = default;
    CfgSound& operator=(CfgSound const&) = delete;
    CfgSound& operator=(CfgSound &&) = default;

	static CfgSound* newEntry(std::string id);
	static CfgSound* fromId(std::string& id);

	std::shared_ptr<base::audio::DtAudioData> rasterize(int rate);

    virtual ~CfgSound();
private:
	explicit CfgSound(std::string& id);

};
};
};
