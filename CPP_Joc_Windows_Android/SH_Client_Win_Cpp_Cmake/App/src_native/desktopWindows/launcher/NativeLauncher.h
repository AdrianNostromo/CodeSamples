#pragma once

#include <GLFW/glfw3.h>
#include <desktopWindows/profilestorage/ProfileStorageNative.h>
#include <desktopWindows/nativeInput/NativeInput.h>
#include <desktopWindows/net/socket/NativeSocketsHandler.h>
#include <desktopWindows/net/http/NativeHttpHandler.h>
#include <desktopWindows/opengl/GLNative.h>
#include <desktopWindows/fileSystem/FileSystemNative.h>
#include <desktopWindows/console/NativeConsole.h>
#include <desktopWindows/audio/native/NativeAudio.h>

class AppBase;

class NativeLauncher final {
private:
	static NativeLauncher* _this;
    
    GLFWwindow* window = nullptr;
    GLNative* gl = nullptr;

	desktopWindows::NativeConsole* nativeConsole = nullptr;

    AppBase* app = nullptr;

    NativeInput* nativeInput = nullptr;

    desktopWindows::ProfileStorageNative* profileStorage;
    desktopWindows::Net::NativeHttpHandler* nativeHttpHandler;

    desktopWindows::Net::NativeSocketsHandler* nativeSocketsHandler;

	// 0; No error.
    // 1; Glfw error.
    int errorCode = 0;

    bool isDirty_windowResolution = true;

protected:
public:
    NativeLauncher();

    int run();

    void reservedDisposeMain();
protected:
    virtual void disposeMain();

private:
	static void onGlfwError(int error, const char* description);
	static void onGlfwWindowsResize(GLFWwindow* window, int resolutionX, int resolutionY);

    void updateScreenMetrics();

};
