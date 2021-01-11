#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector2Int.h>
#include <base/math/Vector2.h>
#include <base/math/Bounds2D.h>

class IListEntry;

namespace rpg3D {
class IGrid2DContentModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual IListEntry*& getGridListEntryReference() = 0;
	
	pub virtual Vector2Int& getGPosRef(bool mustBeInAGCell=true) = 0;
	pub virtual Bounds2D& getGCellBoundsRef(bool mustBeInAGCell = true) = 0;
	pub virtual IListEntry*& getInGCell_listEntryRef() = 0;

	pub ~IGrid2DContentModule() override = default;
};
};
