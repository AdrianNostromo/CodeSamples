#pragma once

#include <base/gh.h>
#include "SVIODH_Base.h"
#include <base/list/ListDL.h>
#include <base/list/ArrayList.h>
#include <memory>

class IEventListener;
namespace base {
    class LoopEvent;
};

namespace base {
namespace snackbar {
class SVIODH_Views : public SVIODH_Base {priv typedef SVIODH_Base super;pub dCtor(SVIODH_Views);
    priv class ViewEntry {pub dCtor(ViewEntry);
        pub IInOrderView* view;

        pub bool doAnimation;

        pub explicit ViewEntry(IInOrderView* view, bool doAnimation)
            : view(view), doAnimation(doAnimation)
        {
            //void
        }
    };

    priv ListDL<ViewEntry> viewsList{};

    priv bool isViewsDirty = false;
    priv float viewShowDelayS = 0.0f;

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit SVIODH_Views(IApp* app);
    prot void create() override;

    pub void addInOrderView(IInOrderView* view, bool doAnimation) final;
    pub void removeInOrderView(IInOrderView* view) final;

    prot void onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event);

    pub ~SVIODH_Views() override;
};
};
};
