#pragma once

namespace base {
namespace audio {
class DtAudioData {
public:
	struct Frame {
		float sample_left;
		float sample_right;
	};
public:
	Frame* framesList;
	int framesCount;

	// This contains some aproximation.
	// The actual duration is given by the framesCount.
	float durationS;

public:
	DtAudioData(DtAudioData const&) = delete;
	DtAudioData(DtAudioData&&) = default;
	DtAudioData& operator=(DtAudioData const&) = delete;
	DtAudioData& operator=(DtAudioData&&) = default;

	explicit DtAudioData(Frame* framesList, int framesCount, float durationS);

	virtual ~DtAudioData();

};
};
};
