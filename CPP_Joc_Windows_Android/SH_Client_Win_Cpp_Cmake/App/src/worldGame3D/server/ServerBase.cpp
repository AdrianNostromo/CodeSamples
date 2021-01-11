#include "ServerBase.h"
#include <base/component/IComponent_ServerComponent.h>

using namespace worldGame3D;

ServerBase::ServerBase(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, worldGame3D::IAppGame* app)
    : super(IComponent_ServerComponent::COMPONENT_CATEGORY),
      logger(logger), errorHandler(errorHandler), profileData(profileData), app(app)
{
    //void
}

ILogHandler* ServerBase::getLogger() {
    return logger;
}

ServerBase::~ServerBase() {
    //void
}
