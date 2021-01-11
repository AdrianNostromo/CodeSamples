#include <base/visual2D/VisualUtil2D.h>
#include <base/visual3d/Visual3DUtil.h>
#include "GPMainBase.h"
#include <base/exceptions/LogicException.h>
#include <graphics/visual3d/drawable/Stage3D.h>
#include <graphics/renderer/Renderer.h>
#include <graphics/environment/environment3D/Environment3D.h>
#include <graphics/renderer/renderPhase/rp3DFinalOutput/RenderPhase3DFinalOutput.h>
#include <graphics/renderer/renderPhase/rp3DShadowMap/RP3DShadowMap.h>
#include <graphics/model/instance/ModelInstance.h>
#include <graphics/model/Model.h>
#include <graphics/model/Node.h>
#include <graphics/model/NodePart.h>
#include <graphics/modelBuilder/ModelBuilder.h>
#include <graphics/material/Material.h>
#include <graphics/material/attributeTypes/EAColorCamera.h>
#include <graphics/model/RenderableLiteInstanceVIMN.h>
#include <graphics/material/Material.h>
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/data/IndexBufferObject.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/util/GraphicsUtil.h>
#include <base/list/CompositesBuffer.h>
#include <graphics/material/attributeTypes/MADirectionalLightsList.h>
#include <graphics/material/attributeTypes/MAPointLightsList.h>
#include <graphics/material/attributeTypes/MASpotLightsList.h>
#include <graphics/material/attributeTypes/MABool.h>
#include <graphics/material/attributeTypes/MAShadowMapLight3D.h>
#include <graphics/material/attributeTypes/EAWorldFocusPosition3D.h>
#include <graphics/material/attributeTypes/light/DirectionalLight.h>
#include <graphics/material/attributeTypes/light/PointLight.h>
#include <graphics/material/attributeTypes/light/SpotLight.h>
#include <graphics/visual/Texture.h>
#include <base/app/IApp.h>
#include <graphics/camera/OrthographicCamera.h>
#include <graphics/camera/PerspectiveCamera.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace base;

std::string GPMainBase::CINEMATIC_CONTROLER_ID = "game_world_main";

GPMainBase::GPMainBase(base::IGameWorld* gw)
    : super(gw)
{
    //void
}

void GPMainBase::createComponents() {
    renderer3D = newt graphics::Renderer();
    renderer3D->reservedCreate();

    graphics::RP3DShadowMap* rp3DShadowMap = newt graphics::RP3DShadowMap(
        0/*renderOrder*/,
        gw->getApp()->getScreen()
    );
    rp3DShadowMap->reservedCreate();
    renderer3D->addRenderPhase(rp3DShadowMap);
    rp3DShadowMap->setIsEnabled(true);

    graphics::RenderPhase3DFinalOutput* rPhase_3DFinalOutput = newt graphics::RenderPhase3DFinalOutput(
        1/*renderOrder*/,
        gw->getApp()->getScreen()
    );
    rPhase_3DFinalOutput->reservedCreate();
    renderer3D->addRenderPhase(rPhase_3DFinalOutput);
    rPhase_3DFinalOutput->setIsEnabled(true);
    
    environment3D = newt graphics::Environment3D();
    environment3D->reservedCreate();

    {
        graphics::EAColorCamera* eaColorCamera = newt graphics::EAColorCamera();

        PerspectiveCamera* camera = newt PerspectiveCamera(74.0f);
        camera->setDistanceRange(0.10f, 15.0f);
        camera->setUp(Vector3::Z);
        eaColorCamera->init(camera);

        environment3D->set(eaColorCamera);
    }

    environment3D->set(graphics::MABool::NewLightingStatus(true));
    environment3D->set(new graphics::EAWorldFocusPosition3D());

    graphics::MAShadowMapLight3D* maShadowMapLight3D = newt graphics::MAShadowMapLight3D(
        2.0f/*distanceRange_min*/, 10.0f/*distanceRange_max*/,
        Vector2(14.0f, 14.0f)/*lightViewPortSize*/
    );
    maShadowMapLight3D->direction.set(-0.3f, 0.2f, -1.0f)->nor();
    maShadowMapLight3D->distToFocusPos = 6.0f;
    environment3D->set(maShadowMapLight3D);

    Vector3 v{};
    {
        graphics::MADirectionalLightsList* maDirectionalLightsList = newt graphics::MADirectionalLightsList();
        environment3D->set(maDirectionalLightsList);

        {
            graphics::DirectionalLight* light = newt graphics::DirectionalLight();

            light->setAmbient(*v.set(0.3f, 0.3f, 0.3f)->mul(1.7f));
            light->setDiffuse(*v.set(0.4f, 0.4f, 0.4f)->mul(1.25f));
            //light->setSpecular(*v.set(0.5f, 0.5f, 0.5f));
            light->setSpecular(*v.set(0.0f, 0.0f, 0.0f));
            
            light->setDirection(*v.set(-0.3f, 0.2f, -1.0f)->nor());

            maDirectionalLightsList->addLightManaged(light);
        }
    }

    // Example lights.
    //{
    //    graphics::MAPointLightsList* maPointLightsList = newt graphics::MAPointLightsList();
    //    environment3D->set(maPointLightsList);

    //    {
    //        graphics::PointLight* light = newt graphics::PointLight();

    //        light->setAmbient(*v.set(0.05f, 0.05f, 0.05f));
    //        light->setDiffuse(*v.set(0.8f, 0.8f, 0.8f));
    //        light->setSpecular(*v.set(1.0f, 1.0f, 1.0f));

    //        light->setPosition(*v.set(1.0f, 1.0f, 1.0f));

    //        light->setTerms(
    //            1.0f/*constant*/,
    //            0.09/*linear*/,
    //            0.032/*quadratic*/
    //        );

    //        maPointLightsList->addLightManaged(light);
    //    }
    //    {
    //        graphics::PointLight* light = newt graphics::PointLight();

    //        light->setAmbient(*v.set(0.05f, 0.05f, 0.05f));
    //        light->setDiffuse(*v.set(0.8f, 0.8f, 0.8f));
    //        light->setSpecular(*v.set(1.0f, 1.0f, 1.0f));

    //        light->setPosition(*v.set(2.0f, 2.0f, 2.0f));

    //        light->setTerms(
    //            1.0f/*constant*/,
    //            0.09/*linear*/,
    //            0.032/*quadratic*/
    //        );

    //        maPointLightsList->addLightManaged(light);
    //    }
    //    {
    //        graphics::PointLight* light = newt graphics::PointLight();

    //        light->setAmbient(*v.set(0.05f, 0.05f, 0.05f));
    //        light->setDiffuse(*v.set(0.8f, 0.8f, 0.8f));
    //        light->setSpecular(*v.set(1.0f, 1.0f, 1.0f));

    //        light->setPosition(*v.set(3.0f, 3.0f, 3.0f));

    //        light->setTerms(
    //            1.0f/*constant*/,
    //            0.09/*linear*/,
    //            0.032/*quadratic*/
    //        );

    //        maPointLightsList->addLightManaged(light);
    //    }
    //    {
    //        graphics::PointLight* light = newt graphics::PointLight();

    //        light->setAmbient(*v.set(0.05f, 0.05f, 0.05f));
    //        light->setDiffuse(*v.set(0.8f, 0.8f, 0.8f));
    //        light->setSpecular(*v.set(1.0f, 1.0f, 1.0f));

    //        light->setPosition(*v.set(4.0f, 4.0f, 4.0f));

    //        light->setTerms(
    //            1.0f/*constant*/,
    //            0.09/*linear*/,
    //            0.032/*quadratic*/
    //        );

    //        maPointLightsList->addLightManaged(light);
    //    }
    //}

    //{
    //    graphics::MASpotLightsList* maSpotLightsList = newt graphics::MASpotLightsList();
    //    environment3D->set(maSpotLightsList);

    //    {
    //        graphics::SpotLight* light = newt graphics::SpotLight();

    //        light->setAmbient(*v.set(0.0f, 0.0f, 0.0f));
    //        light->setDiffuse(*v.set(1.0f, 1.0f, 1.0f));
    //        light->setSpecular(*v.set(1.0f, 1.0f, 1.0f));

    //        light->setPosition(*v.set(1.6f, 4.7f, 4.4f));
    //        light->setDirection(*v.set(0.0f, 0.0f, -1.0f)->nor());

    //        light->setTerms(
    //            1.0f/*constant*/,
    //            0.09/*linear*/,
    //            0.032/*quadratic*/
    //        );

    //        light->setCutOffRad(
    //            Math::cosDegToRad(12.5f)/*cutOff*/,
    //            Math::cosDegToRad(15.0f)/*outterCutOff*/
    //        );

    //        maSpotLightsList->addLightManaged(light);
    //    }
    //}

    if ((cc = environment3D->hookCinematicController(CINEMATIC_CONTROLER_ID)) == nullptr) {
        throw LogicException(LOC);
    }

    stage3D = newt Stage3D();

    layerGameWorld3D = Visual3DUtil::newGroup(stage3D);
}

void GPMainBase::createLayers() {
    visualsMap.putDirectB("gameWorldContainer2D", gameWorldContainer2D = VisualUtil2D::newGroup(nullptr));
}

void GPMainBase::createContent() {
    //void
}

graphics::IRenderer* GPMainBase::getRenderer3D() {
	return renderer3D;
}

void GPMainBase::isPlayEnabled_changedEvent() {
    //void
}

void GPMainBase::disposePre() {
    if(cc != nullptr) {
        if(environment3D->releaseCinematicController(cc) < 0) {
            throw LogicException(LOC);
        }

		cc = nullptr;
    }

    super::disposePre();
}

void GPMainBase::disposeMain() {
    if (environment3D != nullptr) {
		environment3D->reservedDisposeMain();
        delete environment3D;

        environment3D = nullptr;
    }
    if (renderer3D) {
        renderer3D->reservedDisposeMain();
        delete environment3D;

        renderer3D = nullptr;
    }

    if (stage3D) {
        stage3D->reservedDisposeMain();
        delete stage3D;

        stage3D = nullptr;
    }

    super::disposeMain();
}

GPMainBase::~GPMainBase() {
    //void
}
