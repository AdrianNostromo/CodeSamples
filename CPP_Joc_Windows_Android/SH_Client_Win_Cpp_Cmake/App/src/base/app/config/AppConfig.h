#pragma once

#include <base/net/util/IpPort.h>
#include <graphics/visual/Color.h>

class AppConfig {
public:
    static const bool IS_DEBUG;
	static const bool IS_DEBUG_EXTRA_LOGIC_MESSAGES;
	static const bool IS_PHYSICS_DEBUGGER_ENABLED;
	static const bool IS_DEBUG_AssetsLoadDurations;
    // NOTE. This causes lower performance and is only enabled in debug builds even if flag is set.
    /// Use this only to get some extra info when a error is replicatable.
    static const bool IS_DEBUG_ExtraOpenGLErrorsCallback;

    static const bool IS_DEBUG_MEMORY_LEAKS_DETECTION;

    static const int LOCAL_PROFILE_DATA_VERSION;
    static const int CACHE_DATA_VERSION;

    static const float MEDIUM_DEVICE_DIAGONAL_INCHES_MIN;
    static const float LARGE_DEVICE_DIAGONAL_INCHES_MIN;
    static const float LARGE_DEVICE_DPI_SCALE_MAX;

    // This is used to control how the textures are kept in ram (packed or unpacked).
    /// This is currently used to lower the ram ussage for android. May use expanded when multiple fonts are combined into a few textures.
    static const bool usePackedTextureDataInRam;

    static base::Net::IpPort SERVER_ENTRY_POINT_IP_PORT;
    static std::string CLIENT_TYPE;
    static std::string APP_VERSION;

    static Color APP_MAIN_COLOR;

    // Currentlyy these are used for 3d indicators.
    static Color ColorFluorescent_Blue;
    static Color ColorFluorescent_Teal;
    static Color ColorFluorescent_Green;
    static Color ColorFluorescent_Yellow;
    static Color ColorFluorescent_Orange;
    static Color ColorFluorescent_Red;

};
