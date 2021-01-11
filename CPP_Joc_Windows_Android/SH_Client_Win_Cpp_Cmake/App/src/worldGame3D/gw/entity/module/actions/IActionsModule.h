#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <memory>
#include <functional>
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include <base/event/dispatcher/EventDispatcher.h>

namespace worldGame3D {
class IActionsModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* addAction(
		std::shared_ptr<worldGame3D::IAction> newAction,
		ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* preinjectCreatorActionListEntry=nullptr,
		bool addAsDependencyToCreator=false, bool addAsDependantToCreator=false,
		bool triggerFailOnFinishSuccess=false, bool triggerFailOnFail=false) = 0;

	pub virtual base::TypedEventDispatcher1D<worldGame3D::IAction>* getActionsDispatcher() = 0;
	pub virtual std::shared_ptr<IEventListener> setActionProcessor(
		int type,
		std::function<void(IEventListener& eventListener, worldGame3D::IAction& action)> cb) = 0;
	pub virtual void setActionProcessor(
		int type,
		std::function<void(IEventListener& eventListener, worldGame3D::IAction& action)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void setActionProcessor(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, worldGame3D::IAction& action)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void setActionProcessor(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, worldGame3D::IAction& action)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;

	pub ~IActionsModule() override = default;
};
};
