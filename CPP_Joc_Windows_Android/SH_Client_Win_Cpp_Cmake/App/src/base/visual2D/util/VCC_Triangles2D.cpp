#include "VCC_Triangles2D.h"
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

int VCC_Triangles2D::TYPE = super::NewType();

VCC_Triangles2D::VCC_Triangles2D()
    : super(TYPE),
    verticesList(nullptr), indicesManaged(nullptr)
{
    //void
}

VCC_Triangles2D::VCC_Triangles2D(sp<Array1D<ITriangles2DDrawable::LocalVertex>> verticesList, sp<Array1D<unsigned short>> indicesManaged)
    : super(TYPE),
    verticesList(verticesList), indicesManaged(indicesManaged)
{
    //void
}

VCC_Triangles2D* VCC_Triangles2D::New() {
    VCC_Triangles2D* ret = newt VCC_Triangles2D();

    return ret;
}

VCC_Triangles2D* VCC_Triangles2D::New(sp<Array1D<ITriangles2DDrawable::LocalVertex>> verticesList, sp<Array1D<unsigned short>> indicesManaged) {
    VCC_Triangles2D* ret = newt VCC_Triangles2D(verticesList, indicesManaged);

    return ret;
}

VCC_Triangles2D* VCC_Triangles2D::New(graphics::GraphicsUtil::GeneratedGeometry2D geometry) {
    VCC_Triangles2D* ret = newt VCC_Triangles2D(geometry.verticesList, geometry.indicesManaged);

    return ret;
}

VCC_Triangles2D::~VCC_Triangles2D() {
    //void
}
