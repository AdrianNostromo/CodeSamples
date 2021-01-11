#include "AppGame.h"
#include <worldGame3D/gw/config/entity/EntityTemplatesList.h>
#include <base/profiledata/PlayerProfileDataLocal.h>
#include <worldGame3D/app/IAppGame.h>

using namespace worldGame3D;

AppGame::AppGame(
	int deviceType,
	INativeInput* nativeInputConnection,
    base::IProfileStorage* profileStorage,
    base::Net::INativeHttpHandler* nativeHttpHandler,
    base::Net::INativeSocketsHandler* nativeSocketsHandler,
	base::IFileSystem* fileSystem, IGL* gl,
    base::INativeConsole* nativeConsole,
    base::audio::INativeAudio* nativeAudio,
	base::ThreadsManager* threadsManager)
    : super(
		deviceType,
		nativeInputConnection,
        profileStorage,
        nativeHttpHandler,
        nativeSocketsHandler,
        fileSystem, gl,
        nativeConsole,
        nativeAudio,
		threadsManager)
{
    //void
}

void AppGame::create() {
	super::create();
	
	addComponent(server = new Server(
		logger, errorHandler, dynamic_cast<base::IPlayerProfileDataLocal*>(profileData), 
		getHttpRequests(), this,
		nativeSocketsHandler
	), true/*doCreate*/);

	addComponent(scenariosHandler = newUninitialisedScenarioHandler(), true/*doCreate*/, 4/*disposeOrderDefault5*/);
}

void AppGame::createPost(void) {
	super::createPost();
	
	// The app start in offline mode currently.
	server->setIsAccountConnectPrefferedState(false);
}

IServer* AppGame::getServer() {
	return server;
}

ScenariosHandler* AppGame::getScenariosHandler() {
	return scenariosHandler;
}

AppGame::~AppGame() {
    //void
}
