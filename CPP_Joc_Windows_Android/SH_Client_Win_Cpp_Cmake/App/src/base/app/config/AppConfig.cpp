#include "AppConfig.h"

const bool AppConfig::IS_DEBUG = true;
const bool AppConfig::IS_DEBUG_EXTRA_LOGIC_MESSAGES = true;
const bool AppConfig::IS_PHYSICS_DEBUGGER_ENABLED = true;
const bool AppConfig::IS_DEBUG_AssetsLoadDurations = true;
// NOTE. This causes lower performance and is only enabled in debug builds even if flag is set.
/// Use this only to get some extra info when a error is replicatable.
const bool AppConfig::IS_DEBUG_ExtraOpenGLErrorsCallback = false;

const bool AppConfig::IS_DEBUG_MEMORY_LEAKS_DETECTION = false;

const int AppConfig::LOCAL_PROFILE_DATA_VERSION = 5;
// Each cache group has a version and when loaded if the version differs, clear the cache data.
const int AppConfig::CACHE_DATA_VERSION = 1;

const float AppConfig::MEDIUM_DEVICE_DIAGONAL_INCHES_MIN = 7.0f;
const float AppConfig::LARGE_DEVICE_DIAGONAL_INCHES_MIN = 11.0f;
const float AppConfig::LARGE_DEVICE_DPI_SCALE_MAX = 1.5f;

// Use unpacked textures because on Android it has instant maximise (2 packed textures has a maximise time of ~0.7s).
const bool AppConfig::usePackedTextureDataInRam = false;

base::Net::IpPort AppConfig::SERVER_ENTRY_POINT_IP_PORT{"127.0.0.1", 9002};
std::string AppConfig::CLIENT_TYPE = "SQUARE_HEADS";
std::string AppConfig::APP_VERSION = "1.1c";

Color AppConfig::APP_MAIN_COLOR{"526FF2FF"};

// Currentlyy these are used for 3d indicators.
Color AppConfig::ColorFluorescent_Blue{ "006FFFFF" };
Color AppConfig::ColorFluorescent_Teal{ "13F4EFFF" };
Color AppConfig::ColorFluorescent_Green{ "68FF00FF" };
Color AppConfig::ColorFluorescent_Yellow{ "FAFF00FF" };
Color AppConfig::ColorFluorescent_Orange{ "FFBF00FF" };
Color AppConfig::ColorFluorescent_Red{ "FF005CFF" };
