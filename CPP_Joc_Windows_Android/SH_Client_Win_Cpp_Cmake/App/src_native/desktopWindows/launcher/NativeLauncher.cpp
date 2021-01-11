#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>

#include "NativeLauncher.h"
#include <chrono>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <cmath>
#include <base/util/DeviceInfo.h>
#include <base/math/Math.h>
#include <desktopWindows/threadsManager/ThreadsManagerNative.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include <TestLog.h>
#include <base/app/config/AppConfig.h>

#define SQUARE_HEADS
//#define SUDOKU
//#define CALCULATOR

#if defined SQUARE_HEADS
#include <sh/app/AppSquareHeads.h>
#elif defined SUDOKU
#include <sudoku/app/AppSudoku.h>
#elif defined CALCULATOR
#include <calculator/app/AppCalculator.h>
#endif // CALCULATOR

NativeLauncher* NativeLauncher::_this = nullptr;

NativeLauncher::NativeLauncher() {
	NativeLauncher::_this = this;
}

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

int NativeLauncher::run() {
	desktopWindows::ThreadsManagerNative* threadsManager = newt desktopWindows::ThreadsManagerNative();
	// Created in the app.
	//threadsManager->reservedCreate();

	nativeHttpHandler = newt desktopWindows::Net::NativeHttpHandler();
	// Created in the app.
	//nativeHttpHandler->reservedCreate();

    nativeSocketsHandler = newt desktopWindows::Net::NativeSocketsHandler();
	// Created in the app.
	//nativeSocketsHandler->reservedCreate();

	desktopWindows::audio::NativeAudio* nativeAudio = newt desktopWindows::audio::NativeAudio();
	// Created in the app.
	//nativeAudio->reservedCreate();

	if (glfwInit() != GL_TRUE) {
		return -1;
	}

	int glfwVersionMajor;
	int glfwVersionMinor;
	int glfwVersionRev;
	glfwGetVersion(&glfwVersionMajor, &glfwVersionMinor, &glfwVersionRev);
	std::cout << "GLFW Version: " << glfwVersionMajor << "." << glfwVersionMinor << "." << glfwVersionRev << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// Orifukes are definned only for 3.2 and above.
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// This error logic system is slower and only allowed in debug for that reason.
	// If required in release version, manually force it.
	#ifndef NDEBUG
		if (AppConfig::IS_DEBUG && AppConfig::IS_DEBUG_ExtraOpenGLErrorsCallback) {
			std::cout << "WARNING(1). OpenGL extra debug callback is enabled, expect lower performance. This should only be used when a opengl error is investigated." << std::endl;

			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		}
	#endif

	glfwSetErrorCallback(NativeLauncher::onGlfwError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(360, 640, "App", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, NativeLauncher::onGlfwWindowsResize);
	glfwSetWindowPos(window, 950, 85);

    nativeInput = newt NativeInput(window);
    nativeInput->reservedCreate();

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		return -2;
	}

	gl = newt GLNative();
	gl->reservedCreate();

	#ifndef NDEBUG
		if (AppConfig::IS_DEBUG && AppConfig::IS_DEBUG_ExtraOpenGLErrorsCallback) {
			std::cout << "WARNING(2). OpenGL extra debug callback is enabled, expect lower performance. This should only be used when a opengl error is investigated." << std::endl;

			int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
			{
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(glDebugOutput, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			}
		}
	#endif

    gl->getIntegerv(IGL::MAJOR_VERSION, &gl->getCapabilities().majorVersion);
	gl->getIntegerv(IGL::MINOR_VERSION, &gl->getCapabilities().minorVersion);
	gl->getCapabilities().version = std::string(reinterpret_cast<const char*>(gl->getString(IGL::VERSION)));

	gl->getIntegerv(IGL::MAX_UNIFORM_BLOCK_SIZE, &gl->getCapabilities().maxUniformBlockSizeBytes);
	gl->getIntegerv(IGL::UNIFORM_BUFFER_OFFSET_ALIGNMENT, &gl->getCapabilities().uniformBufferOffsetAlignment);
	gl->getIntegerv(IGL::MAX_VERTEX_UNIFORM_BLOCKS, &gl->getCapabilities().maxVertexUniformBlocks);
	gl->getIntegerv(IGL::MAX_FRAGMENT_UNIFORM_BLOCKS, &gl->getCapabilities().maxFragmentUniformBlocks);
	gl->getIntegerv(IGL::MAX_COMBINED_UNIFORM_BLOCKS, &gl->getCapabilities().maxCombinedUniformBlocks);
	gl->getIntegerv(IGL::MAX_VERTEX_UNIFORM_COMPONENTS, &gl->getCapabilities().maxVertexUniformComponents);
	gl->getIntegerv(IGL::MAX_VERTEX_UNIFORM_VECTORS, &gl->getCapabilities().maxVertexUniformVectors);
	gl->getIntegerv(IGL::MAX_FRAGMENT_UNIFORM_VECTORS, &gl->getCapabilities().maxFragmentUniformVectors);
	gl->getIntegerv(IGL::MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, &gl->getCapabilities().maxCombinedVertexUniformComponents);
	gl->getIntegerv(IGL::MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, &gl->getCapabilities().maxCombinedFragmentUniformComponents);

	gl->getIntegerv(IGL::MAX_TEXTURE_IMAGE_UNITS, &gl->getCapabilities().maxTextureImageUnits);
	gl->getIntegerv(IGL::MAX_TEXTURE_SIZE, &gl->getCapabilities().maxTextureSize);
	gl->getIntegerv(IGL::MAX_VERTEX_TEXTURE_IMAGE_UNITS, &gl->getCapabilities().maxVertexTextureImageUnits);
	gl->getIntegerv(IGL::MAX_COMBINED_TEXTURE_IMAGE_UNITS, &gl->getCapabilities().maxCombinedTextureImageUnits);
	
	gl->getIntegerv(IGL::MAX_ELEMENTS_VERTICES, &gl->getCapabilities().maxElementVertices);
	gl->getIntegerv(IGL::MAX_ELEMENTS_INDICES, &gl->getCapabilities().maxElementIndices);

	std::cout << "Device OpenGL version(A): " << gl->getCapabilities().majorVersion << "." << gl->getCapabilities().minorVersion << std::endl;
	std::cout << "Device OpenGL version(B): " << gl->getCapabilities().version << std::endl;
	std::cout << "Device OpenGL glMaxUniformBlockSize: " << gl->getCapabilities().maxUniformBlockSizeBytes << std::endl;

	IGL::Capabilities& capabilitiesCheckVar = gl->getCapabilities();
	(void)capabilitiesCheckVar;

	// This is used for vSinc but is not guaranteed to work correctly.
	// Buffering is implemented in the rendering code so this may not be required.
	// Screen shearing is unlikely on any device (mobile or desktop).
	glfwSwapInterval(1);

    std::string embeddedAssetsFolder = "./assets/";
    std::string privateDataFolder = "./private/";
	FileSystemNative* fileSystem = newt FileSystemNative(
            embeddedAssetsFolder, privateDataFolder
    );
	// Created in the app.
	//fileSystem->reservedCreate();

	profileStorage = newt desktopWindows::ProfileStorageNative();
	// Created in the app.
	//profileStorage->reservedCreate();

	nativeConsole = newt desktopWindows::NativeConsole();

#if defined SQUARE_HEADS
	app = newt sh::AppSquareHeads(
		DeviceInfo::Type::DESKTOP_WINDOWS,
		nativeInput, profileStorage,
		nativeHttpHandler, nativeSocketsHandler,
		fileSystem, gl,
		nativeConsole,
		nativeAudio,
		threadsManager
	);
#elif defined SUDOKU
	app = newt sudoku::AppSudoku(
		DeviceInfo::Type::DESKTOP_WINDOWS,
		nativeInput, profileStorage,
		nativeHttpHandler, nativeSocketsHandler,
		fileSystem, gl,
		nativeConsole,
		nativeAudio,
		threadsManager
	);
#elif defined CALCULATOR
	app = newt calculator::AppCalculator(
		DeviceInfo::Type::DESKTOP_WINDOWS,
		nativeInput, profileStorage,
		nativeHttpHandler, nativeSocketsHandler,
		fileSystem, gl,
		nativeConsole,
		nativeAudio,
		threadsManager
	);
#endif // CALCULATOR

	app->reservedCreate();

	gl->incrementGlContextAndActivate();
	// This is mostly used for android currently.
	app->setIsAppPrefferredActiveStatus(true);

	int targetFrameDurationMs = static_cast<int>(Math::ceil(1000.0f / 60.0f));

	std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
	long lastTickMs = 0;

	while (errorCode == 0 && !glfwWindowShouldClose(window)) {
		if(isDirty_windowResolution) {
			isDirty_windowResolution = false;

			updateScreenMetrics();
		}

		glfwPollEvents();

		// This method is used because it works with large framerates (eg. 1000fps) where the incremental method would have huge rounding errors.
		std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
		auto autoAppRunTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
		long appRunTimeMs = static_cast<int>(autoAppRunTimeMs.count());

		int deltaMs = appRunTimeMs - lastTickMs;
		// NOTE : limit game loop to 10 fps at least.
		deltaMs = std::min(deltaMs, 100);

		targetFrameDurationMs = appRunTimeMs;
		lastTickMs = appRunTimeMs;

		float deltaS = deltaMs / 1000.0f;

        app->mainLoop(deltaS, deltaMs);

		if (app->preRenderLoop(deltaS, deltaMs) != 0) {
			assert(false);
		}

		int r = app->render();
		if (r != 0) {
			assert(false);
		}

		if (TestLog::a.isLogFrame) { TestLog::a.timestampsArray.setDirect(TestLog::a.TimestampSlots::GLSwapBufferPre, std::chrono::system_clock::now()); }

		glfwSwapBuffers(window);

		if (TestLog::a.isLogFrame) { TestLog::a.timestampsArray.setDirect(TestLog::a.TimestampSlots::GLSwapBufferPost, std::chrono::system_clock::now()); }

		int sleepDurationMs = targetFrameDurationMs - deltaMs;
		if (sleepDurationMs > 0) {
			// This is disabled because it causes some frame duration inconsistencies.
			// Vsync seems to be on, so the fps is managed at 60.
			// std::this_thread::sleep_for(std::chrono::milliseconds(sleepDurationMs));
		}
	}

	app->setIsAppPrefferredActiveStatus(false);
	int closeTimerMS = 0;
	while (app->getStatus() != IApp::Status::Innactive) {
		if (closeTimerMS >= 500) {
			throw LogicException(LOC);
		}

		// This method is used because it works with large framerates (eg. 1000fps) where the incremental method would have huge rounding errors.
		std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
		auto autoAppRunTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
		long appRunTimeMs = static_cast<int>(autoAppRunTimeMs.count());

		int deltaMs = appRunTimeMs - lastTickMs;
		// NOTE : limit game loop to 10 fps at least.
		deltaMs = std::min(deltaMs, 100);

		lastTickMs = appRunTimeMs;

		float deltaS = deltaMs / 1000.0f;

		app->mainLoop(deltaS, deltaMs);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		closeTimerMS += 10;
	}

	app->reservedDisposeMain();
	delete app;
	app = nullptr;

	glfwDestroyWindow(window);

	glfwTerminate();

	return errorCode;
}

void NativeLauncher::onGlfwError(int error, const char* description) {
	_this->errorCode = 1;
	std::cout << "Glfw error occured." << description << std::endl;
}

void NativeLauncher::onGlfwWindowsResize(GLFWwindow* window, int resolutionX, int resolutionY) {
	_this->isDirty_windowResolution = true;
}

void NativeLauncher::updateScreenMetrics() {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// NOTE : GLFW Monitors guide is here: http://www.glfw.org/docs/latest/monitor_guide.html
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	// These values are not correct, use manual measurements instead.
	//int widthMM, heightMM;
	//glfwGetMonitorPhysicalSize(monitor, &widthMM, &heightMM);

	// On desktop, glfw can't access the actual monitor density, use the default value it provides and adjustment.
	// Note. These measurement are measured for my actual monitor.
	float measured_screenSizeMM_x = 520.0f;
	float measured_screenSizeMM_y = 295.0f;
	float mmToInchesMultiplier = 1.0f / 25.4f;
	float ppiX = mode->width / (measured_screenSizeMM_x * mmToInchesMultiplier);
	float ppiY = mode->height / (measured_screenSizeMM_y * mmToInchesMultiplier);

	app->setActiveScreenMetrics(
			width, height,
			ppiX, ppiY
	);

	nativeInput->setScreenSize(width, height);
}

void NativeLauncher::reservedDisposeMain() {
    disposeMain();
}

void NativeLauncher::disposeMain() {
	if (app != nullptr) {
		app->reservedDisposeMain();
		delete app;

		app = nullptr;
	}
	if (gl != nullptr) {
		delete gl;

        gl = nullptr;
	}
}
