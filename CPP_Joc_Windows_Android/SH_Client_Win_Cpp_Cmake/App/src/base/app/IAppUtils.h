#pragma once

#include <base/gh.h>
#include <base/input/IInputHandler.h>
#include <base/audio/IAudio.h>
#include <base/profiledata/PlayerProfileData.h>
#include <base/sp.h>

class IContainer2D;
namespace base {
	class IThreadsManager;
};

class IAppUtils {
	pub virtual IInputHandler* getInputHandler() = 0;
	pub virtual base::audio::IAudio* getAudio() = 0;
    
	pub virtual base::IThreadsManager* getThreadsManager() = 0;

	pub virtual base::PlayerProfileData* getProfileData() = 0;

	pub virtual ~IAppUtils() = default;
};
