#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "IGamePartMain.h"
#include <graphics/camera/Camera.h>
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>

class IContainer3D;
class IDrawableNode2D;
namespace graphics {
	class Renderer;
	class Environment3D;
}

namespace base {
class GPMainBase : public GamePart, virtual public IGamePartMain {priv typedef GamePart super;pub dCtor(GPMainBase);
    pub std::shared_ptr<graphics::ICinematicController> cc = nullptr;

    prot graphics::Renderer* renderer3D = nullptr;
    prot Stage3D* stage3D = nullptr;
    prot graphics::Environment3D* environment3D = nullptr;

    prot IContainer3D* layerGameWorld3D = nullptr;

    prot Map1D<std::string, IDrawableNode2D*> visualsMap;
    prot IContainer2D* gameWorldContainer2D = nullptr;

    priv static std::string CINEMATIC_CONTROLER_ID;

    pub explicit GPMainBase(base::IGameWorld* gw);

    pub Stage3D* getStage3D() final {
        return stage3D;
    }
    pub graphics::IRenderer* getRenderer3D() final;
    pub graphics::Environment3D* getEnvironment3D() final {
        return environment3D;
    }
    pub virtual Matrix4 const* getCameraCombined() {
        return cc->peekCamera()->getCombined();
    }

    pub IContainer3D* getLayerGameWorld3D() override {
        return layerGameWorld3D;
    }

    pub std::shared_ptr<graphics::ICinematicController> getCC() override { return cc; }

    pub Map1D<std::string, IDrawableNode2D*>* getVisualsMap() {
        return &visualsMap;
    }

    pub IContainer2D* getGameWorldContainer2D() {
        return gameWorldContainer2D;
    }

    pub virtual void createComponents();
    pub virtual void createLayers();
    pub virtual void createContent();

    priv void tst1();

    pub virtual void isPlayEnabled_changedEvent();

    prot void disposePre() override;
    prot void disposeMain() override;
    pub ~GPMainBase() override;
};
};
