#pragma once

#include <base/gh.h>
#include "VisualCreateConfig.h"
#include <string>
#include <base/assets/util/ManagedAssetPath.h>
#include <base/memory/SharedPointer.h>
#include <graphics/visual2d/drawable/ITriangles2DDrawable.h>
#include <base/list/Array1D.h>
#include <graphics/util/GraphicsUtil.h>

class VCC_Triangles2D : public VisualCreateConfig {priv typedef VisualCreateConfig super;pub dCtor(VCC_Triangles2D);
	pub static int TYPE;

    pub sp<Array1D<ITriangles2DDrawable::LocalVertex>> verticesList;
    pub sp<Array1D<unsigned short>> indicesManaged;
    
    pub explicit VCC_Triangles2D();
    pub explicit VCC_Triangles2D(sp<Array1D<ITriangles2DDrawable::LocalVertex>> verticesList, sp<Array1D<unsigned short>> indicesManaged);

    pub static VCC_Triangles2D* New();
    pub static VCC_Triangles2D* New(sp<Array1D<ITriangles2DDrawable::LocalVertex>> verticesList, sp<Array1D<unsigned short>> indicesManaged);
    pub static VCC_Triangles2D* New(graphics::GraphicsUtil::GeneratedGeometry2D geometry);

    pub virtual ~VCC_Triangles2D();

};
