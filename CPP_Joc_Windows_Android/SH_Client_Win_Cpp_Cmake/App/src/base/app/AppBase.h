#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include <graphics/visual2d/drawable/Stage2D.h>
#include <graphics/camera/Camera.h>
#include <graphics/visual3d/drawable/IContainer3D.h>
#include <graphics/visual2d/drawable/IContainer2D.h>
#include <base/assets/AssetsManager.h>
#include <graphics/visual3d/drawable/Stage3D.h>
#include <base/rendering/RenderingHandler.h>
#include <base/input/InputHandler.h>
#include <base/error/ErrorHandler.h>
#include <base/nativeInput/INativeInput.h>
#include <base/math/Vector2Int.h>
#include <base/profiledata/PlayerProfileData.h>
#include <base/net/http/INativeHttpHandler.h>
#include <base/net/socket/INativeSocketsHandler.h>
#include <base/animator/Animator.h>
#include <base/fileSystem/IFileSystem.h>
#include <base/screen/Screen.h>
#include <base/audio/Audio.h>
#include "IAppInput.h"
#include "IApp.h"
#include <base/audio/native/INativeAudio.h>
#include <base/view/UIView.h>
#include <base/console/INativeConsole.h>
#include <base/log/LogHandler.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include <base/event/dispatcher/EventDispatcher.h>

class IAssetsManager;
namespace base {
    class HttpRequestsManager;
    class ViewsHandler;
    class LoopDistribution;
    class ThreadsManager;
    class RuntimeSettings;
    class ContentHandler;
    namespace snackbar {
        class SnackbarViewsInOrderDisplayHandler;
    };
};
namespace graphics {
    class Renderer;
    class IRenderer;
    class Environment2D;
    class TextureArray;
};
namespace mem {
    class IMemoryManager;
};

class AppBase : public ComponentsHandler, virtual public IApp {priv typedef ComponentsHandler super;pub dCtor(AppBase);
    priv static const bool IS_DEBUG = false;

    // Not created or disposed, it is a global object.
    priv mem::IMemoryManager* memoryManager = nullptr;

    priv RenderingHandler* renderingHandler = nullptr;

    priv Stage2D* stage2D = nullptr;
    priv graphics::Environment2D* environment2D = nullptr;
    priv graphics::Renderer* renderer2D = nullptr;

    priv IContainer2D* layer2D_ui = nullptr;
    
    priv bool isInitialPreloadingFinished = false;
    priv float assetsLoadTimer = 0.0f;
    priv bool isAppLogicStarted = false;

    priv bool isPreloaderViewActivated = false;

    priv float splashScreenActivateTimeS = 0;

    priv base::HttpRequestsManager* httpRequests = nullptr;

    // This is only for local init setting. Use the getComponentAs to access it from other places.
    priv base::ViewsHandler* viewsHandler = nullptr;

    priv base::snackbar::SnackbarViewsInOrderDisplayHandler* snackbarViewsInOrderDisplayHandler = nullptr;
    
    priv base::ContentHandler* contentHandler = nullptr;

    priv base::TypedEventDispatcher1D<base::AppEvent> eventDispatcher{};

    prot AssetsManager* embeddedAssetsManager = nullptr;
    prot base::audio::Audio* audio = nullptr;

    prot LogHandler* logger = nullptr;
    prot ErrorHandler* errorHandler = nullptr;
    prot InputHandler* inputHandler = nullptr;
    prot base::Animator* animator = nullptr;

    prot base::Screen* screen = nullptr;

	// This is device specific data. Not used for much currently. The cache files are used most of the time in the new system.
    prot base::PlayerProfileData* profileData = nullptr;

    prot IContainer2D* layer2DGameWorld = nullptr;

    pub INativeInput* nativeInputConnection;
    pub base::IProfileStorage* profileStorage;
    pub base::Net::INativeHttpHandler* nativeHttpHandler;
    pub base::Net::INativeSocketsHandler* nativeSocketsHandler;
    pub base::IFileSystem* fileSystem;
    pub IGL* gl;
    pub base::INativeConsole* nativeConsole;
    pub base::audio::INativeAudio* nativeAudio;

    pub base::ThreadsManager* threadsManager;
    priv bool isThreadsStopWarningLogged = false;
    priv float threadsManagerStoppingTimerS = 0.0f;

    pub base::LoopDistribution* loopDistribution = nullptr;

    pub base::RuntimeSettings* runtimeSettings = nullptr;

    // This is the general puprpose texture array used to have no texture binds.
    // All textures should be added to this array.
    // This is at texture slot 0.
    // This doesn't allow for textures with pixel art but that should be fine.
    pub ArrayList<graphics::TextureArray*> globalTextures{};

    priv float memoryLeaksDetectionTimerS = 0.0f;

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit AppBase(
		int deviceType,
		INativeInput* nativeInputConnection,
        base::IProfileStorage* profileStorage,
        base::Net::INativeHttpHandler* nativeHttpHandler,
        base::Net::INativeSocketsHandler* nativeSocketsHandler,
		base::IFileSystem* fileSystem, IGL* gl,
        base::INativeConsole* nativeConsole,
        base::audio::INativeAudio* nativeAudio,
        base::ThreadsManager* threadsManager);

    prot void create() override;
    prot void createPost() override;
    pub int createLayers();

    pub bool getIsAppActive() override;

    priv Status status = Innactive;
    priv bool isAppPrefferredActiveStatus = false;
    pub Status getStatus() final;
    priv void setStatus(Status status);
    pub void setIsAppPrefferredActiveStatus(bool isAppPrefferredActiveStatus);

    pub int mainLoop(float deltaS, int deltaMS);
    pub int preRenderLoop(float deltaS, int deltaMS);
    pub int render();

    pub INativeInput* getNativeInput() override;

    pub IAssetsManager* getEmbeddedAssetsManager() final;
    pub Stage2D* getStage2D();
    pub graphics::IRenderer* getRenderer2D();
    
    pub ILogHandler* getLogger() final;
    pub base::IContentHandler* getContentHandler() final;

    pub base::IAnimator* getAnimator() override;

    prot virtual void onLoopEvent_appLoop_General(IEventListener& eventListener, base::LoopEvent& event);

    pub base::IScreen* getScreen() override;

    pub base::PlayerProfileData* getProfileData() override;

    pub RenderingHandler* getRenderingHandler();

    pub IInputHandler* getInputHandler() override;

    pub base::audio::IAudio* getAudio() override;

    pub base::IThreadsManager* getThreadsManager() final;

    pub void setActiveScreenMetrics(
            int width, int height,
            float ppiX, float ppiY);
    pub void setDisableScreen();

    pub base::IHttpRequestsManager* getHttpRequests() override;

    pub base::IRuntimeSettings* getRuntimeSettings() final;

    pub base::ILoopDistribution* getLoopDistribution() final;

    prot virtual base::ViewsHandler* createInstance_ViewsHandler();

    pub base::TypedEventDispatcher1D<base::AppEvent>* getEventDispatcher() override;
    pub std::shared_ptr<IEventListener> addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb) override;
    pub void addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
    pub void addEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
    pub void addEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
    pub void addEventListener(
		ArrayList<int>& eventTypesList,
		std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
    pub int dispatchEvent(base::AppEvent& event) override;

    prot virtual bool tickAndGetIsInitialPreloadFinished(float deltaS);

    prot virtual void onInitialPreloadingFinished();

    prot void disposeMainPre() override;
    prot void disposePost() override;
    pub ~AppBase() override {};
};
