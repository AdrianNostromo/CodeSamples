#pragma once

#include <base/gh.h>

class btRigidBody;
class btCollisionShape;
namespace rpg3D {
	class PBodyConfig;
	class IPBodyUpdateHandler;
};

namespace physics3D {
class ConfiguredPBody {pub dCtor(ConfiguredPBody);
	pub rpg3D::PBodyConfig* config;
	pub btRigidBody* pBody;
	pub btCollisionShape* pShape = nullptr;

	pub rpg3D::IPBodyUpdateHandler* pBodyUpdateHandler = nullptr;
	pub void* ptr_pBodyUpdateHandler = nullptr;

	pub explicit ConfiguredPBody(
		rpg3D::PBodyConfig* config, btRigidBody* pBody, btCollisionShape* pShape,
		rpg3D::IPBodyUpdateHandler* pBodyUpdateHandler, void* ptr_pBodyUpdateHandler);

	pub virtual ~ConfiguredPBody();
};
};
