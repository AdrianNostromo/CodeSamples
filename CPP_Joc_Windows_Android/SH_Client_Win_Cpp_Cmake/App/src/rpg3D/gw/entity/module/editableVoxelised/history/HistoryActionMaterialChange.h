#pragma once

#include <base/gh.h>
#include "HistoryAction.h"
#include <memory>
#include <graphics/visual/Color.h>

namespace rpg3D {
class HistoryActionMaterialChange : public HistoryAction {
private: typedef HistoryAction super;
	pub static int TYPE;

	pub int editorMatIndex;

	pub Color oldMatColor;
	pub Color newMatColor;

public:
   pub dCtor(HistoryActionMaterialChange);
    pub explicit HistoryActionMaterialChange(int editorMatIndex, Color& oldMatColor, Color& newMatColor);

    pub ~HistoryActionMaterialChange() override;
};
};
