#pragma once

#include "AMActionsList.h"

namespace worldGame3D {
class AMProcessorsList : public AMActionsList {
private: typedef AMActionsList super;
private:
	// Note. std::shared_ptr is used here because the current internal map of Map1D uses copy to get entries.
	base::TypedEventDispatcher1D<worldGame3D::IAction> actionsDispatcher{};

public:
    AMProcessorsList(AMProcessorsList const&) = delete;
    AMProcessorsList(AMProcessorsList &&) = default;
    AMProcessorsList& operator=(AMProcessorsList const&) = delete;
    AMProcessorsList& operator=(AMProcessorsList &&) = default;

    explicit AMProcessorsList(
	    IWorldEntity* entity,
	    ActionsTemplate* t);

	base::TypedEventDispatcher1D<worldGame3D::IAction>* getActionsDispatcher() override;
	std::shared_ptr<IEventListener> setActionProcessor(
		int type,
		std::function<void(IEventListener& eventListener, worldGame3D::IAction& action)> cb) override;
	void setActionProcessor(
		int type,
		std::function<void(IEventListener& eventListener, worldGame3D::IAction& action)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
	void setActionProcessor(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, worldGame3D::IAction& action)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
	void setActionProcessor(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, worldGame3D::IAction& action)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;

    ~AMProcessorsList() override;
protected:
	int dispatchAction(worldGame3D::IAction& action);

};
};
