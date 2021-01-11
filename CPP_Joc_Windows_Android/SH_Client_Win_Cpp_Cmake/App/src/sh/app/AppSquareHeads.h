#pragma once

#include <base/gh.h>
#include <sh/test/Tester.h>
#include "IAppSquareHeads.h"
#include <worldGame3D/app/AppGame.h>
#include <base/app/IAppOnline.h>

namespace sh {
class AppSquareHeads final : public worldGame3D::AppGame, virtual public IAppSquareHeads {priv typedef worldGame3D::AppGame super;pub dCtor(AppSquareHeads);
	priv static const bool IS_DEBUG = false;

	//asdA1; priv bool isInitialServerSessionConnected = false;

    priv Tester* tester = nullptr;

    pub explicit AppSquareHeads(
        int deviceType,
        INativeInput* nativeInputConnection,
        base::IProfileStorage* profileStorage,
        base::Net::INativeHttpHandler* nativeHttpHandler,
        base::Net::INativeSocketsHandler* nativeSocketsHandler,
		base::IFileSystem* fileSystem, IGL* gl,
        base::INativeConsole* nativeConsole,
        base::audio::INativeAudio* nativeAudio,
        base::ThreadsManager* threadsManager);
    pub void create() override;
    pub void createPost() override;

    prot base::ViewsHandler* createInstance_ViewsHandler() override;

    prot void onLoopEvent_appLoop_General(IEventListener& eventListener, base::LoopEvent& event) override;

    prot worldGame3D::ScenariosHandler* newUninitialisedScenarioHandler() override;

    prot void onInitialPreloadingFinished() override;

    prot bool tickAndGetIsInitialPreloadFinished(float deltaS) override;

    prot virtual void disposeMain() override;

    pub ~AppSquareHeads() override;
};
};
