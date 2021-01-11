#pragma once

#include <base/list/ArrayList.h>
#include <base/audio/sound/source/generated/part/CfgSoundPart.h>
#include "CfgSoundSource.h"

namespace base {
namespace audio {
class CfgSoundSource_Generated : virtual public CfgSoundSource {
private: typedef CfgSoundSource super;
public:
	static const int TYPE;

	ArrayList<CfgSoundPart*> soundPartsList{};

	int partUidCounter = 0;

public:
    CfgSoundSource_Generated(CfgSoundSource_Generated const&) = delete;
    CfgSoundSource_Generated(CfgSoundSource_Generated &&) = default;
    CfgSoundSource_Generated& operator=(CfgSoundSource_Generated const&) = delete;
    CfgSoundSource_Generated& operator=(CfgSoundSource_Generated &&) = default;

    explicit CfgSoundSource_Generated();

    void addPart(CfgSoundPart* soundPart);

	int computeFramesCount(int rate) override;
	void rasterize(int rate, DtAudioData::Frame* oFramesList, int framesListCount) override;

    ~CfgSoundSource_Generated() override;
};
};
};
