#pragma once

#include <base/menu/IAnimatedComposedStiledVisual.h>
#include <base/gh.h>
#include <string>
#include <memory>
#include <graphics\visual\Color.h>
#include <base/list/ArrayList.h>
#include <base/view/IUIView.h>
#include "listener/IAutoListenerDialogView.h"
#include "util/DynamicTextButtonConfig.h"

namespace base {
class IDialogView : virtual public IUIView {priv typedef IUIView super;
	pub class DynamicTextItem {
		pub dCtor(DynamicTextItem);
		pub std::string text;
		pub int linesCount;

		pub explicit DynamicTextItem(std::string text, int linesCount)
			: text(text), linesCount(linesCount)
		{}
	};
	pub class Config {
		pub dCtor(Config);
		pub float widthDPM;

		pub std::shared_ptr<std::string> titleText;
		pub int titleTotalLinesCount;

		pub std::shared_ptr<DynamicTextItem> dynamicBodyTextItem;

		pub bool isOneButtonControlPerLine;
		pub std::shared_ptr<ArrayList<DynamicTextButtonConfig>> dynamicButtonConfigs;

		pub explicit Config(
			float widthDPM,
			std::shared_ptr<std::string> titleText, int titleTotalLinesCount,
			std::shared_ptr<DynamicTextItem> dynamicBodyTextItem, bool isOneButtonControlPerLine,
			std::shared_ptr<ArrayList<DynamicTextButtonConfig>> dynamicButtonConfigs)
			: widthDPM(widthDPM), titleText(titleText), titleTotalLinesCount(titleTotalLinesCount), dynamicBodyTextItem(dynamicBodyTextItem), isOneButtonControlPerLine(isOneButtonControlPerLine), dynamicButtonConfigs(dynamicButtonConfigs)
		{}
	};

	pub using super::show;
	pub virtual void show(
		bool doAnimation, IAnimatedComposedStiledVisual::IAnimationListener* menuAnimationListener, 
		std::shared_ptr<Config> dialogConfig) = 0;

	pub virtual void setListener(IAutoListenerDialogView* listener) = 0;

	pub virtual ~IDialogView() = default;
};
};
