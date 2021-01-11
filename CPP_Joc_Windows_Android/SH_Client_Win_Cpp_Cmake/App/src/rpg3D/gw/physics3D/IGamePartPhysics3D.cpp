#include "IGamePartPhysics3D.h"
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

int IGamePartPhysics3D::TYPE = GetNew_TYPE();

DEF_componentBasics(IGamePartPhysics3D);

std::string IGamePartPhysics3D::RUNTIME_SETTING_physics3DDebugRender = "physics3DDebugRender";
std::string IGamePartPhysics3D::RUNTIME_SETTING_physics3DDebugClearDepthBuffer = "physics3DDebugClearDepthBuffer";
