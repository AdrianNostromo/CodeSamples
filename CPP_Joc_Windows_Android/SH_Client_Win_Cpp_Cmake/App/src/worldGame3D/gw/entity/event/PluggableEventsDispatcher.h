#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <base/event/dispatcher/IEventDispatcher.h>
#include "IPluggableEventsDispatcher.h"
#include <base/list/ListDL.h>
#include <base/MM.h>

namespace base {
    class GWEvent;
};

namespace worldGame3D {
class PluggableEventsDispatcher final/*Note1001. calls_the_reserved_dispose*/ : public base::Object, virtual public IPluggableEventsDispatcher, public virtual IEventDispatcher {priv typedef base::Object super;pub dCtor(PluggableEventsDispatcher);
	priv struct WrappedListener {
		pub int type;
		pub std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb;
		pub std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected;

		pub EL_Base* childListenerEntry;
		std::shared_ptr<IEventListener> parentAutoListener = nullptr;

		pub explicit WrappedListener(int type, std::function<void(IEventListener& eventListener, base::GWEvent& event)>& cb, EL_Base* childListenerEntry)
			: type(type), cb(cb), childListenerEntry(childListenerEntry)
		{}

		pub void onHandlerDisconnected(IEventListener& eventListener) {
			// This will disonnect this WrappedListener from wrappedListenersList by calling wrappedListenersList(...).
			childListenerEntry->disconnect();
		}

		~WrappedListener() {
			if (parentAutoListener != nullptr) {
				parentAutoListener->disconnect();
				parentAutoListener = nullptr;
			}
		}
	};

	priv ListDL<WrappedListener> wrappedListenersList{LOC};

    priv base::TypedEventDispatcher1D<base::GWEvent>* gwEventDispatcher = nullptr;

    pub explicit PluggableEventsDispatcher();

	pub bool getIsConnected();

    pub void connect(base::TypedEventDispatcher1D<base::GWEvent>* gwEventDispatcher);
    pub void disconnect();

	pub std::shared_ptr<IEventListener> addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected=nullptr) final;
	pub void addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList,
		std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected=nullptr) final;
	pub void addEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList,
		std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected=nullptr) final;
	pub void addEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList,
		std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected=nullptr) final;

	pub void removeListener(EL_Base& eventListener) final;

	pub void dispose() override;
	pub ~PluggableEventsDispatcher() override;
};
};
