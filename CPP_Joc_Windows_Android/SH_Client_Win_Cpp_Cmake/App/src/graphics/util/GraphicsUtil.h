#pragma once

#include <base/gh.h>
#include <base/math/Vector2.h>
#include <base/math/Vector2Int.h>
#include <base/math/Vector3.h>
#include <memory>
#include <graphics/material/Material.h>
#include <graphics/util/GLTypes.h>
#include <base/memory/SharedPointer.h>
#include <graphics/visual2d/drawable/ITriangles2DDrawable.h>
#include <base/list/Array2D.h>
#include <base/list/Array3D.h>
#include <base/map/Map1D.h>
#include <base/math/Bounds2D.h>

namespace graphics {
    class VertexAttributesList;
    class Shader;
};

namespace graphics {
class GraphicsUtil {
    pub class GeneratedGeometry2D {
        pub sp<Array1D<ITriangles2DDrawable::LocalVertex>> verticesList;
        pub sp<Array1D<unsigned short>> indicesManaged;

        pub explicit GeneratedGeometry2D(sp<Array1D<ITriangles2DDrawable::LocalVertex>> verticesList, sp<Array1D<unsigned short>> indicesManaged)
            : verticesList(verticesList), indicesManaged(indicesManaged)
        {};
    };

    priv static constexpr signed char O = -1;
    priv static constexpr signed char W = 0;
    pub static Array2D<signed char> charactersAtlasPixelsGrid2D;
    pub static Array3D<signed char> charactersAtlasPixelsGrid3D;
    pub static Map1D<char, int> characterAtlasOffsetsX;
    pub static Map1D<char, int> characterPixelsWidth;
    priv static void InitCharactersAtlas();
    priv static inline void PushCharInfo(char ch, int charWidth, int* oIndex);

    pub static Shader* CreateFilesSourcesShader(std::string vsPath, std::string fsPath);

    pub static std::shared_ptr<VertexAttributesList> VertexAttributesList_2DGeneric;
    pub static std::shared_ptr<VertexAttributesList> VertexAttributesList_2DGenericTexture;
    pub static std::shared_ptr<VertexAttributesList> VertexAttributesList_3DGenericTexture;
    pub static std::shared_ptr<VertexAttributesList> VertexAttributesList_3DGenericColor;

    pub static std::shared_ptr<graphics::Material> material_2DGenericTexture;
    pub static std::shared_ptr<graphics::Material> material_3DGenericTexture_solid;
    //pub static std::shared_ptr<graphics::Material> material_3DGenericTexture_blended;

    pub static unsigned short* genericIndices;
    pub static int genericIndices_quadsCount;
    pub static int genericIndices_count;

    priv static bool IsStaticsInit;

    //asd_r_1;// add here the texture debug renderer.

    pub static GeneratedGeometry2D GenerateTextGeometry2D(std::string text, float geometryWidth, Vector2 anchor, Vector2 pos, Color col);

    pub static void GenerateTextVoxelsGrid2D(std::string& text, Vector2Int& prevGridSize, Array2D<signed char>* padVoxelGrid=nullptr);
    pub static void GenerateTextVoxelsGrid3D(std::string& text, Vector3Int& prevGridSize, Array3D<signed char>* padVoxelGrid=nullptr);

    // Returns isTriangulationRequired;
    priv static bool BuildTriangulationDistancesGrid2D(Array2D<signed char>& tilingGrid, Array2D<signed char>& oDistancesGridX);
    priv static void TriangulateDistancesGrid2D(
        Array2D<signed char>& distancesGrid, 
        int& oVerticesCount, int& oIndicesCount, Bounds2D& oTrianglesBounds, 
        Vector2& posOffset, float scale, Color& col,
        Array1D<ITriangles2DDrawable::LocalVertex>* verticesList, Array1D<unsigned short>* indicesList);

};
};
