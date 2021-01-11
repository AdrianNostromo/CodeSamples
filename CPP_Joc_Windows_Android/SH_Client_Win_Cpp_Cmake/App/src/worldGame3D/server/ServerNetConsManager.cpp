#include "ServerNetConsManager.h"

using namespace worldGame3D;

ServerNetConsManager::ServerNetConsManager(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, worldGame3D::IAppGame* app)
    : super(logger, errorHandler, profileData, app)
{
    //void
}

INetConsManager* ServerNetConsManager::getNetConsManager() {
    return netConsManager;
}

void ServerNetConsManager::create() {
    super::create();
    
    addComponent(netConsManager = new NetConsManager(this), true/*doCreate*/);
}

ServerNetConsManager::~ServerNetConsManager() {
    //void
}
