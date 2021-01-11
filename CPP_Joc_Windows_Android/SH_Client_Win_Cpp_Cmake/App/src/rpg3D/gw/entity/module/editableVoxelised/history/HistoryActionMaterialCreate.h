#pragma once

#include <base/gh.h>
#include "HistoryAction.h"
#include <memory>
#include <graphics/visual/Color.h>

namespace rpg3D {
class HistoryActionMaterialCreate : public HistoryAction {
private: typedef HistoryAction super;
	pub static int TYPE;

	pub int editorMatIndex;

	pub Color diffuseColor;

public:
   pub dCtor(HistoryActionMaterialCreate);
    pub explicit HistoryActionMaterialCreate(int editorMatIndex, Color& diffuseColor);

    pub ~HistoryActionMaterialCreate() override;
};
};
