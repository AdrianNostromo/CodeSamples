#pragma once

#include <base/gh.h>
#include <base/menu/IAnimatedComposedStiledVisual.h>
#include <memory>
#include <base/map/ExtraData.h>
#include <base/math/Vector3.h>

class WorldEntity;
class IWorldEntity;
class EntityTemplate;
class EntityRemoveReason;

namespace rpg3D {
class IEntityEditorView : virtual public IAnimatedComposedStiledVisual {
	pub class EditorType {
		// Note: "OTHER" entity = not self entity.

		pub static const int EDIT_OTHER;
		pub static const int PAINT_OTHER;
	};

	pub virtual ~IEntityEditorView() = default;
};
};
