#pragma once

#include <base/gh.h>
#include "HistoryAction.h"
#include <memory>
#include <graphics/visual/Color.h>

namespace rpg3D {
class HistoryActionMaterialRemove : public HistoryAction {
private: typedef HistoryAction super;
	pub static int TYPE;

	pub int editorMatIndex;

	pub Color& col;

public:
   pub dCtor(HistoryActionMaterialRemove);
    pub explicit HistoryActionMaterialRemove(int editorMatIndex, Color& col);

    pub ~HistoryActionMaterialRemove() override;
};
};
