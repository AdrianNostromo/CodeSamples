#include "ServerPart.h"

using namespace worldGame3D;

ServerPart::ServerPart(IServerPartHandler* server, IApp* app)
    : super(),
      server(server), app(app)
{
    //void
}

ServerPart::~ServerPart() {
    //void
}
