#pragma once

#include <base/gh.h>
#include <string>
#include <functional>
#include <memory>
#include <graphics/visual/Color.h>
#include <base/menu/IAnimatedComposedStiledVisual.h>
#include <base/list/ArrayList.h>
#include <base/view/IUIView.h>
#include <base/object/IWrappedValue.h>
#include <base/map/Map1D.h>
#include <base/view/listener/AutoListener.h>

namespace base {
	class BitmapLabel;
};

namespace base {
class IColorSelectorView : virtual public IUIView {priv typedef IUIView super;
public:
	class DynamicTextButtonConfig {
	public:
		int id;

		std::string text;
		Color tintColor;
		float alpha;

	public:
		DynamicTextButtonConfig(DynamicTextButtonConfig const&) = delete;
		DynamicTextButtonConfig(DynamicTextButtonConfig&&) = default;
		DynamicTextButtonConfig& operator=(DynamicTextButtonConfig const&) = delete;
		DynamicTextButtonConfig& operator=(DynamicTextButtonConfig&&) = default;

		explicit DynamicTextButtonConfig(
			int id,
			std::string text, Color& tintColor, float alpha)
			: id(id),
			text(text), tintColor(tintColor), alpha(alpha)
		{}

		virtual ~DynamicTextButtonConfig() {};
	};
	class IListener : virtual public IAutoListener {
	public:
		virtual void onDynamicButtonActivated(IColorSelectorView* view, DynamicTextButtonConfig* dynamicButtonConfig, Color& activeColor) = 0;
		virtual void onCloseRequested(IColorSelectorView* view) = 0;

		virtual ~IListener() = default;
	};
	class CbListener : public AutoListener, virtual public IListener {
	public:
		std::function<void(
			IColorSelectorView* view, DynamicTextButtonConfig* dynamicButtonConfig, Color& activeColor)> cb_onDynamicButtonActivated;

		std::function<void(
			IColorSelectorView* view)> cb_onCloseRequested;

	public:
		CbListener(CbListener const&) = delete;
		CbListener(CbListener&&) = default;
		CbListener& operator=(CbListener const&) = delete;
		CbListener& operator=(CbListener&&) = default;

		explicit CbListener();

		void onDynamicButtonActivated(IColorSelectorView* view, DynamicTextButtonConfig* dynamicButtonConfig, Color& activeColor) override;
		void onCloseRequested(IColorSelectorView* view) override;

		~CbListener() override;
	};
public:
	using super::show; 
	virtual void show(bool doAnimation, IAnimatedComposedStiledVisual::IAnimationListener* menuAnimationListener, Color* initialColor, std::shared_ptr<ArrayList<DynamicTextButtonConfig>> dynamicButtonConfigs, bool isAlphaEditable, int alphaOverrideHexValue) = 0;
	
	pub virtual void setListener(IListener* listener) = 0;

	virtual Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* getUserDataMap() = 0;

	virtual void updateControlLblText(int controlIndex, std::string newText) = 0;

    virtual ~IColorSelectorView() = default;
};
};	
