#include "ConfiguredPBody.h"

using namespace physics3D;

ConfiguredPBody::ConfiguredPBody(
	rpg3D::PBodyConfig* config, btRigidBody* pBody, btCollisionShape* pShape,
	rpg3D::IPBodyUpdateHandler* pBodyUpdateHandler, void* ptr_pBodyUpdateHandler)
	: config(config), pBody(pBody), pShape(pShape),
	pBodyUpdateHandler(pBodyUpdateHandler), ptr_pBodyUpdateHandler(ptr_pBodyUpdateHandler)
{
	// void
}

ConfiguredPBody::~ConfiguredPBody() {
	//void
}
