#pragma once

#include <base/menu/IAnimatedComposedStiledVisual.h>
#include <graphics\visual\Color.h>
#include <memory>
#include <graphics/visual2d/drawable/List2D.h>

namespace base {
class IListMenuCardPopupView : virtual public IAnimatedComposedStiledVisual {
public:
	class IListener {
	public:
		virtual void onListEntryActivated(IListMenuCardPopupView* targetView, List2D* list, ListEntry2D* listEntry) = 0;
		virtual void onCloseRequested(IListMenuCardPopupView* targetView) = 0;

		virtual ~IListener() = default;
	};
	class CbListener : virtual public IListener {
	public:
		std::function<void(
			IListMenuCardPopupView* targetView, List2D* list, ListEntry2D* listEntry)> cb_onListEntryActivated;
		
		std::function<void(
			IListMenuCardPopupView* targetView)> cb_onCloseRequested;

	public:
		CbListener(CbListener const&) = delete;
		CbListener(CbListener&&) = default;
		CbListener& operator=(CbListener const&) = delete;
		CbListener& operator=(CbListener&&) = default;

		explicit CbListener();

		void onListEntryActivated(IListMenuCardPopupView* targetView, List2D* list, ListEntry2D* listEntry) override;
		void onCloseRequested(IListMenuCardPopupView* targetView) override;

		~CbListener() override;
	};
	class Config {
	public:
		float posXDip;
		float posYDip;

		float widthDip;

		bool isFullScreenDimmer;

		Color bgColor;

	public:
		Config(float posXDip, float posYDip, float widthDip, bool isFullScreenDimmer, Color& bgColor)
			: posXDip(posXDip), posYDip(posYDip),
			widthDip(widthDip),
			isFullScreenDimmer(isFullScreenDimmer),
			bgColor(bgColor)
		{
			//void
		}

	};
public:
	virtual void show(bool doAnimation, IAnimationListener* menuAnimationListener, std::shared_ptr<Config> viewConfig, IListener* listener) = 0;

	virtual ListEntry2D* createNewListEntry(int index, ArrayList<MenuItemConfig*>* itemInstanceConfigOverride, ArrayList<StateChangeDurations*>* easeDurationsSList, Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* extraDataMap) = 0;

    virtual ~IListMenuCardPopupView() = default;
};
};
