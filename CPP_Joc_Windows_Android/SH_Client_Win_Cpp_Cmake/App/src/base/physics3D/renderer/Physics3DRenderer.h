#pragma once

#include <base/gh.h>
#include <base/physics3D/renderer/shapeDrawer/Physics3DShapeDrawer.h>
#include <base/rendering/util/IRenderableRenderer.h>
#include <string>
#include <memory>

class IApp;
class btDiscreteDynamicsWorld;
namespace graphics {
    class Shader;
    class ICinematicController;
};

namespace physics3D {
class Physics3DRenderer : public virtual IRenderableRenderer {pub dCtor(Physics3DRenderer);
    priv static std::string u_colorCameraProjectionViewMatrix;

    pub Physics3DShapeDrawer shapeDrawer{};

    prot IApp* app;

    priv btDiscreteDynamicsWorld* pWorld;
    priv std::shared_ptr<graphics::ICinematicController> cinematicController;

    priv graphics::Shader* shader = nullptr;

    priv bool isClearDepthBuffer = false;

    pub explicit Physics3DRenderer(IApp* app, btDiscreteDynamicsWorld* pWorld, std::shared_ptr<graphics::ICinematicController> cinematicController);
    pub void create();

    pub void setIsClearDepthBuffer(bool isClearDepthBuffer);

    pub void prepareForRendering(graphics::IEnvironment* environment) final;
    pub void render(graphics::IEnvironment* environment) final;

    pub ~Physics3DRenderer() override;
};
};
