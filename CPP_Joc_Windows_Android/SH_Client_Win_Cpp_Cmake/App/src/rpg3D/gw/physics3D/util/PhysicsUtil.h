#pragma once

#include <base/gh.h>

namespace rpg3D {
class PhysicsUtil {
	// Collission margins ussage increases performance and gives perfectly mathematic rounded edges beveling.
	// Bullet may not calculate an accurate inertia tensor when a margin interraction occurs. Inertia tensor scaling may fix this problem.
	/// This occurs if eg. a sphere is dropped on an angled shape.

	// Primitive shapes and compound of them are more stable than triangle meshes (This may be more usefull for dynamic bodies).

	// The Collission_margin is an extruded rounded shape. The faces are extruded using the face normal and then are connected by perfectly rounded edges.
	
	// Spheres convert their size to collission margins and don't require any to be set.
	// Box, cilinder, capsule have collission margins that are inside the created pShape, manual pShape shrinking not required.
	// Cone, Convex_hull have an exterior collission margin. Manual pShape shrinking required.
	
	pub static const float DEFAULT_COLLISSION_MARGIN;
	// This should be used for small BOX shapes.
	pub static const float SMALL_COLLISSION_MARGIN;
};
};
