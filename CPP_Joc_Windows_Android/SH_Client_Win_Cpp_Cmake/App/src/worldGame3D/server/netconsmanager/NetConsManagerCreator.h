#pragma once

#include "NetConsManagerBase.h"

namespace worldGame3D {
class NetConsManagerCreator : public NetConsManagerBase {
private: typedef NetConsManagerBase super;
public:
    NetConsManagerCreator(NetConsManagerCreator const&) = delete;
    NetConsManagerCreator(NetConsManagerCreator &&) = default;
    NetConsManagerCreator& operator=(NetConsManagerCreator const&) = delete;
    NetConsManagerCreator& operator=(NetConsManagerCreator &&) = default;

    explicit NetConsManagerCreator(IServerPartHandler* server);

    virtual ~NetConsManagerCreator();
};
};
