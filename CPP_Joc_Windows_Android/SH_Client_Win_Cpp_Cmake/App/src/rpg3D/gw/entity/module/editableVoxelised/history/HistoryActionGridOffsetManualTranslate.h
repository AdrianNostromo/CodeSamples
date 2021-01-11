#pragma once

#include <base/gh.h>
#include "HistoryAction.h"
#include <base/math/Vector3.h>

namespace rpg3D {
class HistoryActionGridOffsetManualTranslate : public HistoryAction {
private: typedef HistoryAction super;
	pub static int TYPE;

	pub Vector3 translate;

public:
   pub dCtor(HistoryActionGridOffsetManualTranslate);
    pub explicit HistoryActionGridOffsetManualTranslate(Vector3& translate);

    pub ~HistoryActionGridOffsetManualTranslate() override;
};
};
