#pragma once

#include <base/gh.h>
#include <string>
#include <graphics/visual/Color.h>

namespace base {
namespace snackbar {
class DynamicTextButtonConfig {pub dCtor(DynamicTextButtonConfig);
	pub int id;

	pub std::string text;
	pub Color tintColor;
	pub float alpha;

	pub bool hasExclusiveLine;

	pub explicit DynamicTextButtonConfig(
		int id,
		std::string text, Color& tintColor, float alpha,
		bool hasExclusiveLine);

	pub virtual ~DynamicTextButtonConfig();
};
};
};
