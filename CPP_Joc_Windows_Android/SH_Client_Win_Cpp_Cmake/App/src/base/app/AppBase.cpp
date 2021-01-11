// This should be the first include here.
#include <base/statics/StaticsInit.h>

#include "AppBase.h"
#include <assert.h>
#include <base/opengl/util/GLUtil.h>
#include <base/visual2D/VisualUtil2D.h>
#include <graphics/visual2d/drawable/Sprite2D.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <string>
#include <base/math/Vector3.h>
#include <base/math/Math.h>
#include <base/assets/util/ManagedAssetPath.h>
#include <base/exceptions/LogicException.h>
#include <base/app/config/AppConfig.h>
#include <base/buffer/util/BitsUtil.h>
#include <base/crypto/cipher/IEncryptCipher.h>
#include <base/crypto/cipher/IDecryptCipher.h>
#include <base/crypto/util/CryptoUtil.h>
#include <base/profiledata/config/ProfileDataIDs.h>
#include <base/util/StringUtil.h>
#include <graphics/visual/BitmapFont.h>
#include <base/exceptions/ValueNotFoundException.h>
#include <base/app/event/AppEvent.h>
#include <graphics/environment/environment2D/Environment2D.h>
#include <base/util/DeviceInfo.h>
#include <base/profiledata/PlayerProfileDataLocal.h>
#include <base/httprequests/HttpRequestsManager.h>
#include <base/appLoop/LoopDistribution.h>
#include <base/threadsManager/ThreadsManager.h>
#include <graphics/renderer/Renderer.h>
#include <graphics/renderer/renderPhase/rp2DFinalOutput/RenderPhase2DFinalOutput.h>
#include <base/memory/IMemoryManager.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <graphics/visual/TextureArray.h>
#include <graphics/model/NodePart.h>
#include <graphics/model/RenderableLiteInstanceVIMN.h>
#include <graphics/geometry/data/MeshPart.h>
#include <graphics/geometry/Mesh.h>
#include <graphics/material/Material.h>
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/data/IndexBufferObject.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/util/GraphicsUtil.h>
#include <base/list/CompositesBuffer.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <graphics/visual/Texture.h>
#include <base/MM.h>
#include <graphics/material/attributeTypes/EAColorCamera.h>
#include <graphics/camera/OrthographicCamera.h>
#include <base/runtimeSettings/RuntimeSettings.h>
#include <graphics/geometry/data/FrameBufferObject.h>
#include <TestLog.h>
#include <base/view/snackbar/inOrderDisplay/SnackbarViewsInOrderDisplayHandler.h>
#include <base/view/splashScreen/SplashScreenView.h>
#include <base/viewsHandler/ViewsHandler.h>
#include <base/component/IComponent_AppComponent.h>
#include <base/contentHandler/ContentHandler.h>

AppBase::AppBase(
	int deviceType,
	INativeInput* nativeInputConnection,
    base::IProfileStorage* profileStorage,
    base::Net::INativeHttpHandler* nativeHttpHandler,
    base::Net::INativeSocketsHandler* nativeSocketsHandler,
	base::IFileSystem* fileSystem, IGL* gl,
    base::INativeConsole* nativeConsole,
    base::audio::INativeAudio* nativeAudio,
    base::ThreadsManager* threadsManager)
	: super(IComponent_AppComponent::COMPONENT_CATEGORY),
     nativeInputConnection(nativeInputConnection),
     profileStorage(profileStorage),
     nativeHttpHandler(nativeHttpHandler), nativeSocketsHandler(nativeSocketsHandler),
     fileSystem(fileSystem), gl(gl),
      nativeConsole(nativeConsole),
      nativeAudio(nativeAudio),
    threadsManager(threadsManager)
{
	DeviceInfo::deviceType = deviceType;
}

void AppBase::create() {
    memoryManager = mem::MM::mm;

    base::StaticsInit::InitStatics();

    GLUtil::gl->setCachedClearColorDirect(Color{ "#000000FF" });
    
    super::create();
	
    {
        // Pixel art textures.
        graphics::TextureArray* ta_nearest = newt graphics::TextureArray(
            0/*uIndex*/,
            1024/*width*/, 1024/*height*/, 4/*channels*/ * 8/*bits_per_pixel*/,
            &Texture::TextureFilterMin::Nearest/*minFilter*/, &Texture::TextureFilterMag::Nearest/*magFilter*/,
            &Texture::TextureWrap::ClampToEdge/*wrapS*/, &Texture::TextureWrap::ClampToEdge/*wrapT*/,
            0/*minTexturesCapacity*/, graphics::TextureArray::TextureBindSlots::globalTexArray_nearest/*hardcodedTextureBindSlot*/
        );
        ta_nearest->reservedCreate();
        globalTextures.appendDirect(ta_nearest);
        // Manually increment these here because only the tex_arrays that are created here are in the global group, others can be custom created in other places for different purposes.
        graphics::TextureArray::GLOBAL_TEXTURE_ARRAYS_COUNT++;
        
        // Filtered textures.
        graphics::TextureArray* ta_linear = newt graphics::TextureArray(
            1/*uIndex*/,
            1024/*width*/, 1024/*height*/, 4/*channels*/ * 8/*bits_per_pixel*/,
            &Texture::TextureFilterMin::Linear/*minFilter*/, &Texture::TextureFilterMag::Linear/*magFilter*/,
            &Texture::TextureWrap::ClampToEdge/*wrapS*/, &Texture::TextureWrap::ClampToEdge/*wrapT*/,
            0/*minTexturesCapacity*/, graphics::TextureArray::TextureBindSlots::globalTexArray_linear/*hardcodedTextureBindSlot*/
        );
        ta_linear->reservedCreate();
        globalTextures.appendDirect(ta_linear);
        // Manually increment these here because only the tex_arrays that are created here are in the global group, others can be custom created in other places for different purposes.
        graphics::TextureArray::GLOBAL_TEXTURE_ARRAYS_COUNT++;

        graphics::GraphicsUtil::material_2DGenericTexture->getMustExistAs<graphics::MADiffuseTexturesArray*>(graphics::MADiffuseTexturesArray::TypeBitGroup)->globalTextures = &globalTextures;
        graphics::GraphicsUtil::material_3DGenericTexture_solid->getMustExistAs<graphics::MADiffuseTexturesArray*>(graphics::MADiffuseTexturesArray::TypeBitGroup)->globalTextures = &globalTextures;
        //asd_0x; graphics::GraphicsUtil::material_3DGenericTexture_blended->getMustExistAs<graphics::MADiffuseTexturesArray*>(graphics::MADiffuseTexturesArray::TypeBitGroup)->texturesArray = mainTextureArray0;

        // This should be the only manual texture bind in the entire app.
        // mainTextureArray0->bind();// Don't bind here because the gl context doesn't exist yet. It will be bound when textures are loaded.
    }

    logger = newt LogHandler(nativeConsole);
    logger->reservedCreate();

    threadsManager->reservedCreate();

    fileSystem->initVars(this);
    fileSystem->reservedCreate();

    profileStorage->init(this);
    profileStorage->reservedCreate();

    nativeHttpHandler->init(this);
    nativeHttpHandler->reservedCreate();

    nativeSocketsHandler->init(this);
    nativeSocketsHandler->reservedCreate();

    nativeAudio->init(this);
    nativeAudio->reservedCreate();

    errorHandler = newt ErrorHandler(logger);
    errorHandler->reservedCreate();

    // Create the player profile object. It uses the localDataHandler to read and save key, baseValue pairs.
    profileStorage->loadDataMap();

    profileData = newt base::PlayerProfileDataLocal(logger, profileStorage);
    if(profileData->getDataWithID_int(base::ProfileDataIDs::PROFILE_DATA_VERSION, -1) != AppConfig::LOCAL_PROFILE_DATA_VERSION) {
        profileData->clearAllData();
        
        profileData->putDataWithID_int(base::ProfileDataIDs::PROFILE_DATA_VERSION, AppConfig::LOCAL_PROFILE_DATA_VERSION);
    }

    loopDistribution = newt base::LoopDistribution();

    addComponent(inputHandler = newt InputHandler(this, nativeInputConnection, errorHandler), true/*doCreate*/);
    addComponent(animator = newt base::Animator(), true/*doCreate*/);

    addComponent(audio = newt base::audio::Audio(this, nativeAudio), true/*doCreate*/);

    unsigned int errorCode = GLUtil::gl->getError();
	assert(errorCode == GLUtil::gl->NO_ERROR);

	addComponent(httpRequests = newt base::HttpRequestsManager(this, nativeHttpHandler), true/*doCreate*/);
	
    // Dispose the views later because the debug view requires all debug entries users to disconnect their entries before the view can be disposed (maybe change that logic if needed).
    addComponent(viewsHandler = createInstance_ViewsHandler(), true/*doCreate*/, 6/*disposeOrderDefault5*/);

	addComponent(snackbarViewsInOrderDisplayHandler = newt base::snackbar::SnackbarViewsInOrderDisplayHandler(this), true/*doCreate*/);
    viewsHandler->snackbarViewsInOrderDisplayHandler = snackbarViewsInOrderDisplayHandler;

    addComponent(contentHandler = newt base::ContentHandler(this), true/*doCreate*/);

	embeddedAssetsManager = newt AssetsManager(this);
	embeddedAssetsManager->reservedCreate();
    embeddedAssetsManager->initGlobalTextures(&globalTextures);

	screen = newt base::Screen();
	screen->reservedCreate();

    renderingHandler = newt RenderingHandler();
    graphics::FrameBufferObject::DefaultFBO = new graphics::FrameBufferObject(true/*isDefaultFBO*/);
    graphics::FrameBufferObject::DefaultFBO->reservedCreate();

    runtimeSettings = newt base::RuntimeSettings();
    runtimeSettings->reservedCreate();

    {
        environment2D = newt graphics::Environment2D();
        environment2D->reservedCreate();

        {
            graphics::EAColorCamera* eaColorCamera = newt graphics::EAColorCamera();

            OrthographicCamera* camera = newt OrthographicCamera(false);
            eaColorCamera->init(camera);

            environment2D->set(eaColorCamera);
        }
    }

    renderer2D = newt graphics::Renderer();
	renderer2D->reservedCreate();

    graphics::RenderPhase2DFinalOutput* rPhase_2DFinalOutput = newt graphics::RenderPhase2DFinalOutput(
        0/*renderOrder*/, screen
    );
    rPhase_2DFinalOutput->reservedCreate();
    renderer2D->addRenderPhase(rPhase_2DFinalOutput);
    rPhase_2DFinalOutput->setIsEnabled(true);
    
	//asdRV2;// Set the newt renderer camera.
	// The camera is used for touch events unprojecting to usefull coords.
    stage2D = newt Stage2D(environment2D, inputHandler);
    stage2D->reservedCreate();

	int i = createLayers();
    if(i != 0) {
        throw LogicException(LOC);
    }

    renderingHandler->addRenderable(
        getStage2D(), environment2D, getRenderer2D(), getRenderer2D(),
        IRenderingHandler::RenderOrderGroup::ROG_2D, IRenderingHandler::RendererProfilerType::RPT_2D
    );
    
    getLoopDistribution()->addEventListener(
        base::ILoopDistribution::AppLoopEvent_gameWorld_pre->type,
        std::bind(&AppBase::onLoopEvent_appLoop_General, this, std::placeholders::_1, std::placeholders::_2),
        localAutoListenersList
    );
}

void AppBase::setActiveScreenMetrics(
        int width, int height,
        float ppiX, float ppiY)
{
    if (width == 0 || height == 0) {
        // This is required because size of 0 will cause cursor camera unprojection to create a 3d ray that has NaN values because the values get divided by this width and height and become NaN.
        width = height = 1;
    }

    //asd_g_t_1;// Test fon't size changes
    /*static bool asd_s_ = false;
    asd_s_ = !asd_s_;
    if (asd_s_) {
        ppiX += 110.0f;
        ppiY += 110.0f;
    }*/

	screen->setActiveScreenMetrics(
		width, height,
		ppiX, ppiY
	);
}

void AppBase::setDisableScreen() {
	screen->setDisableScreen();
}

void AppBase::createPost(void) {
	super::createPost();

    //void
}

int AppBase::createLayers() {
    // All layers are created here because multiple different drawers exist.
    layer2D_ui = VisualUtil2D::newGroup(stage2D);

    viewsHandler->layer2DGameWorld = layer2DGameWorld = VisualUtil2D::newGroup(stage2D);
    viewsHandler->layerSidebar = VisualUtil2D::newGroup(stage2D);
    viewsHandler->layer2D_debug = VisualUtil2D::newGroup(stage2D);
    viewsHandler->layerDialogs = VisualUtil2D::newGroup(stage2D);
    viewsHandler->layerStatusView = VisualUtil2D::newGroup(stage2D);

    viewsHandler->layer2D_splashScreen = VisualUtil2D::newGroup(stage2D);

    return 0;
}

bool AppBase::getIsAppActive() {
    return status == Active ? true : false;
}

AppBase::Status AppBase::getStatus() {
    return status;
}

void AppBase::setStatus(Status status) {
    if (this->status == status) {
        throw LogicException(LOC);
    }

    bool isAppActive_old = this->status == Active ? true : false;
    bool isAppActive_new = status == Active ? true : false;

    if (isAppActive_old != isAppActive_new) {
        dispatchEvent(*Event_isAppActiveChangePre);
    }

    if (status == Innactive) {
        GlobalAccessLogHandler::devLogHandler->post_debug("App. New status: Innactive");

        if (this->status != Inactivating) {
            throw LogicException(LOC);
        }

        profileStorage->flush();
        fileSystem->flush();
    } else if (status == Inactivating) {
        GlobalAccessLogHandler::devLogHandler->post_debug("App. New status: Inactivating");

        if (this->status != Active) {
            throw LogicException(LOC);
        }

        // Stop native threads.
        threadsManagerStoppingTimerS = 0.0f;
        isThreadsStopWarningLogged = false;
        threadsManager->setIsNatveThreadsPrefferedEnabled(false);
    } else if (status == Active) {
        GlobalAccessLogHandler::devLogHandler->post_debug("App. New status: Active");

        if (this->status != Innactive) {
            throw LogicException(LOC);
        }

        threadsManager->setIsNatveThreadsPrefferedEnabled(true);
    } else {
        throw LogicException(LOC);
    }
    this->status = status;

    if (isAppActive_old != isAppActive_new) {
        dispatchEvent(*Event_isAppActiveChanged);
    }
}

void AppBase::setIsAppPrefferredActiveStatus(bool isAppPrefferredActiveStatus) {
    if (this->isAppPrefferredActiveStatus == isAppPrefferredActiveStatus) {
        return;
    }
    this->isAppPrefferredActiveStatus = isAppPrefferredActiveStatus;

    if (!this->isAppPrefferredActiveStatus && status == Active) {
        setStatus(Inactivating);
    }else if (this->isAppPrefferredActiveStatus && status == Innactive) {
        setStatus(Active);
    }
}

int AppBase::mainLoop(float deltaS, int deltaMS) {
    TestLog::a.logFrameCounter--;
    if (TestLog::a.logFrameCounter <= 0) {
        TestLog::a.logFrameCounter = 60;

        TestLog::a.frameStartTimestamp = std::chrono::system_clock::now();

        TestLog::a.isLogFrame = true;
    } else {
        TestLog::a.isLogFrame = false;
    }

    if (AppConfig::IS_DEBUG && AppConfig::IS_DEBUG_MEMORY_LEAKS_DETECTION) {
        memoryLeaksDetectionTimerS += deltaS;
        if (memoryLeaksDetectionTimerS >= 2.0f) {
            memoryLeaksDetectionTimerS = 0.0f;
            
            mem::MM::mm->logLargestDeltaAllocations();
            mem::MM::mm->startDeltaAllocationsSnapshot();
        }
    }

    if (status == Active) {
        std::chrono::time_point<std::chrono::system_clock> tm1 = std::chrono::system_clock::now();//asd_01;_t;_r;
        loopDistribution->distributeLoop(deltaS, deltaMS);
        
        threadsManager->manageThreadsLoop();

        if (!isAppPrefferredActiveStatus) {
            setStatus(Inactivating);
        }
    } else if (status == Inactivating) {
        // Wait for the native thread to exit.
        threadsManager->manageThreadsLoop();

        if (threadsManager->getNativeThreadsStatus() == base::IThreadsManager::NativeThreadsStatus::Off) {
            status = Innactive;
        } else {
            threadsManagerStoppingTimerS += deltaS;
            // Use a large limit with a warning to avoid app crashes when de device is under load.
            if(threadsManagerStoppingTimerS > 0.5f && !isThreadsStopWarningLogged) {
                isThreadsStopWarningLogged = true;

                // Threads stop should have finished, log a warning.
                GlobalAccessLogHandler::devLogHandler->post_warning("App. Native threads stop exceeded the 0.5s warning duration.");
            }
            if (threadsManagerStoppingTimerS >= 5.0f) {
                throw LogicException(LOC);
            }
        }
    } else if (status == Innactive) {
        if (isAppPrefferredActiveStatus) {
            setStatus(Active);
        }
    }

    if (TestLog::a.isLogFrame) { TestLog::a.timestampsArray.setDirect(TestLog::a.TimestampSlots::MainLoopEnd, std::chrono::system_clock::now()); }

	return 0;
}

int AppBase::preRenderLoop(float deltaS, int deltaMS) {
    if (TestLog::a.isLogFrame) { TestLog::a.timestampsArray.setDirect(TestLog::a.TimestampSlots::PreRenderStart, std::chrono::system_clock::now()); }
    
    animator->loopTickPreRendering(deltaS, deltaMS);

	return 0;
}

int AppBase::render() {
    // Viewport is set by each render phase.
    //GLUtil::gl->viewport(0, 0, screen->getWindowResolutionX(), screen->getWindowResolutionY());
    GLUtil::gl->clear(IGL::DEPTH_BUFFER_BIT | IGL::COLOR_BUFFER_BIT);

    renderingHandler->render();

    unsigned int errorCode = GLUtil::gl->getError();
    if (errorCode != IGL::NO_ERROR) {
        return -2;
    }
    
    if (TestLog::a.isLogFrame) { TestLog::a.timestampsArray.setDirect(TestLog::a.TimestampSlots::RenderEnd, std::chrono::system_clock::now()); }

	return 0;
}

INativeInput* AppBase::getNativeInput() {
    return nativeInputConnection;
}

IAssetsManager* AppBase::getEmbeddedAssetsManager() {
    return embeddedAssetsManager;
}

Stage2D* AppBase::getStage2D() {
    return stage2D;
}

graphics::IRenderer* AppBase::getRenderer2D() {
    return renderer2D;
}

ILogHandler* AppBase::getLogger() {
    return logger;
}

base::IContentHandler* AppBase::getContentHandler() {
    return contentHandler;
}

base::IAnimator* AppBase::getAnimator() {
    return animator;
}

void AppBase::onLoopEvent_appLoop_General(IEventListener& eventListener, base::LoopEvent& event) {
    if(!isPreloaderViewActivated) {
        isPreloaderViewActivated = true;

        //embeddedAssetsManager->preloadAssetB(AssetType::TextureAtlas, "visual2D/taSplashScreen.atlas");
        
        embeddedAssetsManager->preloadAssetB(AssetType::Text, "shaders/default2D.frag");
        embeddedAssetsManager->preloadAssetB(AssetType::Text, "shaders/default2D.vert");
        embeddedAssetsManager->preloadAssetB(AssetType::Text, "shaders/default3D.frag");
        embeddedAssetsManager->preloadAssetB(AssetType::Text, "shaders/default3D.vert");

        assetsLoadTimer = 0.0f;
        while (embeddedAssetsManager->isAssetsPreloading()) {
            if (embeddedAssetsManager->preloadTick() != 0) {
                assert(false);
            }

            assetsLoadTimer += 1.0f;
            if(assetsLoadTimer > 200.0f) {
                // Assets preload took too much time.
                throw LogicException(LOC);
            }
        }
        if(embeddedAssetsManager->isAnyFailedLoaders()) {
            throw LogicException(LOC);
        }

        if(AppConfig::IS_DEBUG && IS_DEBUG) {
            logger->postInfo("Preload(Initial phase) - Start.");
        }

        viewsHandler->getSplashScreenView()->show(false, nullptr, nullptr);
        splashScreenActivateTimeS = 0;

        if(ManagedAssetPath::isInitialPreloadStarted) {
            assert(false);
        }
        ManagedAssetPath::isInitialPreloadStarted = true;

        embeddedAssetsManager->preloadManagedAssets(*ManagedAssetPath::get_entriesToLoadList());
        ManagedAssetPath::get_entriesToLoadList()->clear();
        assetsLoadTimer = 0.0f;
    }
    if(!isAppLogicStarted) {
        splashScreenActivateTimeS += event.deltaS;

        if(tickAndGetIsInitialPreloadFinished(event.deltaS) && splashScreenActivateTimeS >= 1.2f) {
            isAppLogicStarted = true;

            onInitialPreloadingFinished();
        }
    }

	audio->loopTick();
}

void AppBase::onInitialPreloadingFinished() {
    if(AppConfig::IS_DEBUG && IS_DEBUG) {
        logger->postInfo("Preload(Initial phase) - All finished.");
    }

    viewsHandler->hideAndAutoDisposeSplashScreen();
    
    viewsHandler->initDebugView();
}

bool AppBase::tickAndGetIsInitialPreloadFinished(float deltaS) {
    if(!isInitialPreloadingFinished) {
        if (embeddedAssetsManager->preloadTick() != 0) {
            assert(false);
        }

        if(!embeddedAssetsManager->isAssetsPreloading()) {
            if(embeddedAssetsManager->isAnyFailedLoaders()) {
                throw LogicException(LOC);
            }

            isInitialPreloadingFinished = true;

            if(AppConfig::IS_DEBUG && IS_DEBUG) {
                logger->postInfo("Preload(Initial phase) - Assets finished.");
            }
        }

        assetsLoadTimer += deltaS;
        if(assetsLoadTimer > 15.0f) {
            // Assets preload took too much time.
            throw LogicException(LOC);
        }
    }

    return isInitialPreloadingFinished;
}

base::IScreen* AppBase::getScreen() {
    return screen;
}

base::PlayerProfileData* AppBase::getProfileData() {
	return profileData;
}

RenderingHandler* AppBase::getRenderingHandler() {
    return renderingHandler;
}

IInputHandler* AppBase::getInputHandler() {
    return inputHandler;
}

base::audio::IAudio* AppBase::getAudio() {
	return audio;
}

base::IThreadsManager* AppBase::getThreadsManager() {
    return threadsManager;
}

base::IHttpRequestsManager* AppBase::getHttpRequests() {
	return httpRequests;
}

base::IRuntimeSettings* AppBase::getRuntimeSettings() {
    return runtimeSettings;
}

base::ILoopDistribution* AppBase::getLoopDistribution() {
    return loopDistribution;
}

base::ViewsHandler* AppBase::createInstance_ViewsHandler() {
    return newt base::ViewsHandler(this);
}

base::TypedEventDispatcher1D<base::AppEvent>* AppBase::getEventDispatcher() {
	return &eventDispatcher;
}

std::shared_ptr<IEventListener> AppBase::addEventListener(
	int type,
	std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb)
{
	return eventDispatcher.getDispatcher(type, true)->addListener(cb);
}

void AppBase::addEventListener(
	int type,
	std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    base::EventDispatcher<base::AppEvent>* dispatcher = eventDispatcher.getDispatcher(type, true);
    dispatcher->addListener(cb, &autoListenersList);
}

void AppBase::addEventListener(
	int type1, int type2,
	std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	addEventListener(type1, cb, autoListenersList);
	addEventListener(type2, cb, autoListenersList);
}

void AppBase::addEventListener(
	int type1, int type2, int type3,
	std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	addEventListener(type1, cb, autoListenersList);
	addEventListener(type2, cb, autoListenersList);
	addEventListener(type3, cb, autoListenersList);
}

void AppBase::addEventListener(
	ArrayList<int>& eventTypesList,
	std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	for (int i = 0; i < eventTypesList.count(); i++) {
		int type = eventTypesList.getDirect(i);

		addEventListener(type, cb, autoListenersList);
	}
}

int AppBase::dispatchEvent(base::AppEvent& event) {
	int ct = 0;

	base::EventDispatcher<base::AppEvent>* d = eventDispatcher.getDispatcher(event.type, false);
	if (d != nullptr) {
		ct += d->dispatch(event);
	}

    event.clearData();

	return ct;
}

void AppBase::disposeMainPre() {
    //void

    super::disposeMainPre();
}

void AppBase::disposePost() {
    if (runtimeSettings != nullptr) {
        runtimeSettings->reservedDispose();
        delete runtimeSettings;
        runtimeSettings = nullptr;
    }
    if(layer2D_ui) {
        layer2D_ui->reservedDisposeMain();
        delete layer2D_ui;

        layer2D_ui = nullptr;
    }
    if(renderer2D) {
        renderer2D->reservedDisposeMain();
        delete renderer2D;

        renderer2D = nullptr;
    }
    if(environment2D) {
		environment2D->reservedDisposeMain();
        delete environment2D;

		environment2D = nullptr;
    }
    if(renderingHandler) {
        delete renderingHandler;

        renderingHandler = nullptr;
    }
    if(screen) {
	    screen->reservedDisposeMain();
        delete screen;

	    screen = nullptr;
    }
    
    if (nativeAudio) {
        nativeAudio->reservedDisposeMain();
        delete nativeAudio;
        nativeAudio = nullptr;
    }
    
    if (profileStorage) {
        profileStorage->flush();

        profileStorage->reservedDisposeMain();
        delete profileStorage;
        profileStorage = nullptr;
    }
    if (profileData) {
        //profileData->reservedDisposeMain();
        delete profileData;
        profileData = nullptr;
    }
    
    if (fileSystem) {
        fileSystem->flush();

        fileSystem->reservedDisposeMain();
        delete fileSystem;
        fileSystem = nullptr;
    }

    if (threadsManager) {
        // Call the loop from here to remove any remaining finished threads that require removing.
        threadsManager->manageThreadsLoop();
        threadsManager->reservedDisposeMain();
        delete threadsManager;
        threadsManager = nullptr;
    }

	super::disposePost();
}
