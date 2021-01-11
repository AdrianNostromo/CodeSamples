#include "ServerComponentIpPort.h"
#include <worldGame3D/app/IAppGame.h>

using namespace worldGame3D;

//// Gets the ip, ports for server components. Uses an component id system.
ServerComponentIpPort::ServerComponentIpPort(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, worldGame3D::IAppGame* app)
    : super(logger, errorHandler, profileData, app)
{
    //void
}

void ServerComponentIpPort::create() {
    super::create();
    
    addComponent(componentIpPort = new ComponentIpPort(this, app), true/*doCreate*/);
}

IComponentIpPort* ServerComponentIpPort::getComponentIpPort() {
    return componentIpPort;
}

ServerComponentIpPort::~ServerComponentIpPort() {
    //void
}
