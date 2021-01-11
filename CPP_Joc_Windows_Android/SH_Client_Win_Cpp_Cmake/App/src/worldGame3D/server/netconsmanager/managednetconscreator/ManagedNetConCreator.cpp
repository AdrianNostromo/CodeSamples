#include "ManagedNetConCreator.h"

using namespace worldGame3D;

ManagedNetConCreator::ManagedNetConCreator(IServerPartHandler* server, std::string& serverComponentId, IListener* listener)
    : super(server, serverComponentId, listener)
{
    //void
}

ManagedNetConCreator::~ManagedNetConCreator() {
    //void
}
