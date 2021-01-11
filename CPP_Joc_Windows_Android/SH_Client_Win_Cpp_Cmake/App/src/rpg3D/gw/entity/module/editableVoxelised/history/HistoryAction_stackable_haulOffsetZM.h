#pragma once

#include <base/gh.h>
#include "HistoryAction.h"
#include <base/math/Vector3.h>

namespace rpg3D {
class HistoryAction_stackable_haulOffsetZM : public HistoryAction {
private: typedef HistoryAction super;
	pub static int TYPE;

	pub float new_haulOffsetZM;
	pub float old_haulOffsetZM;

public:
   pub dCtor(HistoryAction_stackable_haulOffsetZM);
    pub explicit HistoryAction_stackable_haulOffsetZM(
		float new_haulOffsetZM, float old_haulOffsetZM);

    pub ~HistoryAction_stackable_haulOffsetZM() override;
};
};
