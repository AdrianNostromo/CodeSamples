#pragma once

#include <base/app/AppBase.h>
#include <worldGame3D/scenariohandler/ScenariosHandler.h>
#include "IAppGame.h"
#include <worldGame3D/server/Server.h>

namespace worldGame3D {
class AppGame : public AppBase, virtual public IAppGame {priv typedef AppBase super;pub dCtor(AppGame);
	priv ScenariosHandler* scenariosHandler = nullptr;

	priv Server* server = nullptr;

	pub explicit AppGame(
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

	prot virtual ScenariosHandler* newUninitialisedScenarioHandler() = 0;
	pub ScenariosHandler* getScenariosHandler() final;

	pub IServer* getServer() override;

    pub ~AppGame() override;
};
};
