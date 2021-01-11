#pragma once

class DeviceInfo {
public:
    class Type {
    public:
        static const int DESKTOP_LINUX;
        static const int DESKTOP_WINDOWS;
        // ANDROID is a reserved word, use MOBILE_ANDROID instead.
        static const int MOBILE_ANDROID;
    };
public:
    // This is initialised by MainLocal constructor.
    static int deviceType;

};
