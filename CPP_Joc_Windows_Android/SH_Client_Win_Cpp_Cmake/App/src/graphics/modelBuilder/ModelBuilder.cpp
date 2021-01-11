#include "ModelBuilder.h"
#include <graphics/model/Model.h>
#include <graphics/model/Node.h>
#include <graphics/model/NodePart.h>
#include <graphics/geometry/Mesh.h>
#include <graphics/geometry/data/MeshPart.h>
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/data/IndexBufferObject.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/util/GraphicsUtil.h>
#include "shape/ShapeBuilderBox.h"
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace graphics;

Node* ModelBuilder::CreateCubeNode(
    float size,
    std::shared_ptr<graphics::Material> material0, Color& vertexAttributeColor,
    Model* targetModel,
    Vector3* vertexTranslation)
{
    return CreateBoxNode(
        size, size, size,
        material0, vertexAttributeColor,
        targetModel,
        vertexTranslation
    );
}

Node* ModelBuilder::CreateBoxNode(
    float sizeX, float sizeY, float sizeZ,
    std::shared_ptr<graphics::Material> material0, Color& vertexAttributeColor,
    Model* targetModel,
    Vector3* vertexTranslation)
{
    graphics::Node* node0 = newt graphics::Node(targetModel == nullptr ? true : false/*computeNormalTransform*/);

    std::shared_ptr<VertexAttributesList> vertexAttributesList = GraphicsUtil::VertexAttributesList_3DGenericColor;

    graphics::VertexBufferObject* vbo = newt graphics::VertexBufferObject(vertexAttributesList, graphics::VertexBufferObject::UpdateMethod::SingleRegion);
    graphics::IndexBufferObject* ibo = newt graphics::IndexBufferObject(graphics::VertexBufferObject::UpdateMethod::SingleRegion);

    ShapeBuilderBox::DrawCube(
        vbo, ibo,
        sizeX, sizeY, sizeZ,
        vertexAttributeColor,
        vertexTranslation
    );

    vbo->reservedCreate();
    ibo->reservedCreate();

    graphics::Mesh* mesh0 = newt graphics::Mesh(
        vbo,
        ibo
    );

    graphics::MeshPart* meshPart0 = newt graphics::MeshPart(
        mesh0, 
        0/*verticesOffset*/, vbo->getBuffer().count()/*verticesCount*/,
        0/*indicesOffset*/, ibo->getBuffer().count()/*indicesCount*/
    );
    
    graphics::NodePart* nodePart0 = newt graphics::NodePart(meshPart0, material0);
    node0->appendNodePart(nodePart0);

    if (targetModel != nullptr) {
        targetModel->appendMesh(node0->partsList.getReference(0)->meshPart->mesh);

        targetModel->appendNode(node0);
    }

    return node0;
}
