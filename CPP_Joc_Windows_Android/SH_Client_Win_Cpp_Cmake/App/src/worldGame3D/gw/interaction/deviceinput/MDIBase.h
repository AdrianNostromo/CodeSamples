#pragma once

#include <base/input/util/BlockingInputLayer.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "IManagedDeviceInput.h"

class IAppUtils;

namespace base {
class MDIBase : virtual public IManagedDeviceInput {
protected:
    IGameWorld* gw;
    IAppUtils* appUtils;

public:
    MDIBase(MDIBase const&) = delete;
    MDIBase(MDIBase &&) = default;
    MDIBase& operator=(MDIBase const&) = delete;
    MDIBase& operator=(MDIBase &&) = default;

    explicit MDIBase(IGameWorld* gw);
    void reservedCreate();
    void reservedCreateB();

    void reservedDisposePre();
    void reservedDispose();

    ~MDIBase() override;
protected:
    virtual void createMain();
    virtual void createBMain();

    virtual void disposePre();
    virtual void disposeMain();
};
};
