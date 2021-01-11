#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include <base/component/IComponent_AppComponent.h>
#include <base/list/ArrayList.h>
#include <memory>

class IEventListener;
namespace base {
    class LoopEvent;
};
namespace sh {
	class IAppSquareHeads;
};

namespace sh {
class TBase : public base::ManagedObject, public virtual IComponent_AppComponent {priv typedef base::ManagedObject super;pub dCtor(TBase);
    prot IAppSquareHeads* app;

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit TBase(IAppSquareHeads* app);
    pub void create() override;

    prot virtual void onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event);

    priv void tst1();
    priv void tst2();
    priv void tst3();

    pub ~TBase() override;
};
};
