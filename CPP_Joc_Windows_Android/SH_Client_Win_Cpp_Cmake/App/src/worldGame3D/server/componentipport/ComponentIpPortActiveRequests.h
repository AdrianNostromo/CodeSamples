#pragma once

#include <base/gh.h>
#include "ComponentIpPortBase.h"
#include <base/list/ArrayList.h>

class IEventListener;
namespace base {
    class LoopEvent;
};

namespace worldGame3D {
class ComponentIpPortActiveRequests : public ComponentIpPortBase, virtual public ComponentIpPortBase::Request::IHandler {priv typedef ComponentIpPortBase super;pub dCtor(ComponentIpPortActiveRequests);
    priv ArrayList<std::shared_ptr<Request>> newRequestsList{};
    priv ArrayList<std::shared_ptr<Request>> activeRequestList{};
    priv ArrayList<std::shared_ptr<Request>> finishedRequestsList{};

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit ComponentIpPortActiveRequests(IServerPartHandler* handler, IApp* app);
    prot void create() override;

    pub std::shared_ptr<IRequest> poolServerComponentIpPort(
        std::string& serverComponentId,
        IRequestListener* listener
    ) override;

    prot virtual void onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event);

    priv void onRequestHttpFinished(Request* target) override;
    priv void onRequestCancelRequested(Request* target) override;

    priv void moveActiveRequestPointerToFinishedList(Request* target);

    pub ~ComponentIpPortActiveRequests() override;
};
};
