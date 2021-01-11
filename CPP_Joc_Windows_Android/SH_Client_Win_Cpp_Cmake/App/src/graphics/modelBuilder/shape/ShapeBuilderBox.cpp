#include "ShapeBuilderBox.h"
#include <graphics/model/Model.h>
#include <graphics/model/Node.h>
#include <graphics/model/NodePart.h>
#include <graphics/geometry/Mesh.h>
#include <graphics/geometry/data/VertexBufferObject.h>
#include <graphics/geometry/data/IndexBufferObject.h>
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <graphics/visual/Color.h>

using namespace graphics;

void ShapeBuilderBox::DrawCube(
    graphics::VertexBufferObject* vbo, graphics::IndexBufferObject* ibo,
    float sizeX, float sizeY, float sizeZ, Color& vertexAttributeColor,
    Vector3* vertexTranslation)
{
    float pLX = -sizeX / 2.0f;
    float pHX = sizeX / 2.0f;
    float pLY = -sizeY / 2.0f;
    float pHY = sizeY / 2.0f;
    float pLZ = -sizeZ / 2.0f;
    float pHZ = sizeZ / 2.0f;
    if (vertexTranslation != nullptr) {
        pLX += vertexTranslation->x;
        pHX += vertexTranslation->x;
        pLY += vertexTranslation->y;
        pHY += vertexTranslation->y;
        pLZ += vertexTranslation->z;
        pHZ += vertexTranslation->z;
    }

    graphics::GLTypes::Col4 col{ vertexAttributeColor.r, vertexAttributeColor.g, vertexAttributeColor.b, vertexAttributeColor.a };
    // HZ
    DrawQuad(
        vbo, ibo,
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pLX, pLY, pHZ),
            graphics::GLTypes::Vect3(0.0f, 0.0f, 1.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pLX, pHY, pHZ),
            graphics::GLTypes::Vect3(0.0f, 0.0f, 1.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pHX, pHY, pHZ),
            graphics::GLTypes::Vect3(0.0f, 0.0f, 1.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pHX, pLY, pHZ),
            graphics::GLTypes::Vect3(0.0f, 0.0f, 1.0f),
            col,
            0u/*a_modelTransformIndex*/
        }
    );

    // LZ
    DrawQuad(
        vbo, ibo,
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pLX, pLY, pLZ),
            graphics::GLTypes::Vect3(0.0f, 0.0f, -1.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pHX, pLY, pLZ),
            graphics::GLTypes::Vect3(0.0f, 0.0f, -1.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pHX, pHY, pLZ),
            graphics::GLTypes::Vect3(0.0f, 0.0f, -1.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pLX, pHY, pLZ),
            graphics::GLTypes::Vect3(0.0f, 0.0f, -1.0f),
            col,
            0u/*a_modelTransformIndex*/
        }
    );

    // HX
    DrawQuad(
        vbo, ibo,
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pHX, pLY, pLZ),
            graphics::GLTypes::Vect3(1.0f, 0.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pHX, pLY, pHZ),
            graphics::GLTypes::Vect3(1.0f, 0.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pHX, pHY, pHZ),
            graphics::GLTypes::Vect3(1.0f, 0.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pHX, pHY, pLZ),
            graphics::GLTypes::Vect3(1.0f, 0.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        }
    );

    // LX
    DrawQuad(
        vbo, ibo,
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pLX, pLY, pLZ),
            graphics::GLTypes::Vect3(-1.0f, 0.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pLX, pHY, pLZ),
            graphics::GLTypes::Vect3(-1.0f, 0.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pLX, pHY, pHZ),
            graphics::GLTypes::Vect3(-1.0f, 0.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pLX, pLY, pHZ),
            graphics::GLTypes::Vect3(-1.0f, 0.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        }
    );

    // HY
    DrawQuad(
        vbo, ibo,
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pLX, pHY, pLZ),
            graphics::GLTypes::Vect3(0.0f, 1.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pHX, pHY, pLZ),
            graphics::GLTypes::Vect3(0.0f, 1.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pHX, pHY, pHZ),
            graphics::GLTypes::Vect3(0.0f, 1.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pLX, pHY, pHZ),
            graphics::GLTypes::Vect3(0.0f, 1.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        }
    );

    // LY
    DrawQuad(
        vbo, ibo,
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pLX, pLY, pLZ),
            graphics::GLTypes::Vect3(0.0f, -1.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pLX, pLY, pHZ),
            graphics::GLTypes::Vect3(0.0f, -1.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pHX, pLY, pHZ),
            graphics::GLTypes::Vect3(0.0f, -1.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        },
        graphics::GLTypes::Vertex_3DGenericColor{
            graphics::GLTypes::Vect3(pHX, pLY, pLZ),
            graphics::GLTypes::Vect3(0.0f, -1.0f, 0.0f),
            col,
            0u/*a_modelTransformIndex*/
        }
    );
}

void ShapeBuilderBox::DrawQuad(
    graphics::VertexBufferObject* vbo, graphics::IndexBufferObject* ibo,
    graphics::GLTypes::Vertex_3DGenericColor v0, graphics::GLTypes::Vertex_3DGenericColor v1, graphics::GLTypes::Vertex_3DGenericColor v2, graphics::GLTypes::Vertex_3DGenericColor v3)
{
    unsigned short firstIndex = vbo->getBuffer().count();

    vbo->getBuffer().reserveInsertCapacity(4/*vertices*/);
    *((graphics::GLTypes::Vertex_3DGenericColor*)vbo->getBuffer().appendUninitialised()) = v0;
    *((graphics::GLTypes::Vertex_3DGenericColor*)vbo->getBuffer().appendUninitialised()) = v1;
    *((graphics::GLTypes::Vertex_3DGenericColor*)vbo->getBuffer().appendUninitialised()) = v2;
    *((graphics::GLTypes::Vertex_3DGenericColor*)vbo->getBuffer().appendUninitialised()) = v3;

    ibo->getBuffer().reserveInsertCapacity(6);
    ibo->getBuffer().appendUShort(firstIndex + 0);
    ibo->getBuffer().appendUShort(firstIndex + 3);
    ibo->getBuffer().appendUShort(firstIndex + 2);

    ibo->getBuffer().appendUShort(firstIndex + 0);
    ibo->getBuffer().appendUShort(firstIndex + 2);
    ibo->getBuffer().appendUShort(firstIndex + 1);
}
