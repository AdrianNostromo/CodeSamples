#pragma once

#include <base/gh.h>
#include <base/app/IAppAssets.h>
#include <base/app/IAppMetrics.h>
#include <base/app/IAppUtils.h>
#include <base/app/IAppInput.h>
#include "IAppOnline.h"
#include <base/log/ILogHandler.h>
#include <base/animator/IAnimator.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <functional>
#include <memory>
#include <base/list/ArrayList.h>
#include <base/component/IComponentsHandler.h>

class IEventListener;
namespace base {
	class AppEvent;
	class IRuntimeSettings;
	class IContentHandler;
}
namespace base {
	class ILoopDistribution;
}

class IApp : public virtual IComponentsHandler, virtual public IAppAssets, virtual public IAppOnline, virtual public IAppMetrics, virtual public IAppUtils, virtual public IAppInput {
	pub static base::AppEvent* Event_isAppActiveChangePre;
	pub static base::AppEvent* Event_isAppActiveChanged;
	// This is used to instantly stop editors.

	pub enum Status { Innactive, Inactivating, Active };
	pub virtual Status getStatus() = 0;

	pub virtual ILogHandler* getLogger() = 0;
	pub virtual base::IContentHandler* getContentHandler() = 0;

	pub virtual bool getIsAppActive() = 0;

	pub virtual base::IRuntimeSettings* getRuntimeSettings() = 0;

	pub virtual base::ILoopDistribution* getLoopDistribution() = 0;

	pub virtual base::TypedEventDispatcher1D<base::AppEvent>* getEventDispatcher() = 0;
	pub virtual std::shared_ptr<IEventListener> addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb) = 0;
	pub virtual void addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		ArrayList<int>& eventTypesList,
		std::function<void(IEventListener& eventListener, base::AppEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual int dispatchEvent(base::AppEvent& event) = 0;

	pub virtual ~IApp() override = default;
};
