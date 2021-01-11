#include "GraphicsUtil.h"
#include <graphics/geometry/vertexAttribute/VertexAttributesList.h>
#include <base/statics/StaticsInit.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <graphics/material/attributeTypes/MAInt.h>
#include <graphics/material/attributeTypes/MABlending.h>
#include <graphics/material/attributeTypes/EAColorCamera.h>
#include <graphics/material/attributeTypes/MACastShadow.h>
#include <graphics/material/attributeTypes/MACastColor.h>
#include <graphics/material/attributeTypes/MAReceiveShadow.h>
#include <graphics/material/attributeTypes/MADepthTest.h>
#include <base/opengl/IGL.h>
#include <graphics/shader/Shader.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>

using namespace graphics;

Array2D<signed char> GraphicsUtil::charactersAtlasPixelsGrid2D{
    315, 5,
    false, true,
    new signed char[315 * 5] {
        O,W,W,O,O,W,W,W,O,O,O,W,W,W,O,W,W,W,O,O,W,W,W,W,O,W,W,W,W,O,O,W,W,O,O,W,O,O,W,O,W,W,W,O,O,O,W,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,O,W,O,W,O,O,W,O,O,W,W,O,O,W,W,W,O,O,O,W,W,O,O,O,W,W,W,O,O,O,W,W,W,O,W,W,W,W,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,W,W,W,O,O,W,W,O,O,W,W,W,O,O,O,W,W,W,O,W,W,W,O,O,W,W,W,W,O,W,W,W,W,O,O,W,W,O,O,W,O,O,W,O,W,W,W,O,O,O,W,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,O,W,O,W,O,O,W,O,O,W,W,O,O,W,W,W,O,O,O,W,W,O,O,O,W,W,W,O,O,O,W,W,W,O,W,W,W,W,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,W,W,W,O,O,O,W,O,W,W,W,O,O,W,W,W,O,O,O,O,W,W,O,O,W,W,W,O,O,W,W,O,O,W,W,W,W,O,O,W,W,O,O,O,W,W,O,O,O,W,W,O,O,
        W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,W,O,O,O,O,W,O,O,O,O,W,O,O,O,O,W,O,O,W,O,O,W,O,O,O,O,O,W,O,W,O,W,O,O,W,O,O,O,O,W,W,O,W,W,O,W,W,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,W,O,O,W,O,O,W,O,W,O,O,O,O,O,O,W,O,O,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,W,O,W,W,O,W,O,W,O,O,W,O,O,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,W,O,O,O,O,W,O,O,O,O,W,O,O,O,O,W,O,O,W,O,O,W,O,O,O,O,O,W,O,W,O,W,O,O,W,O,O,O,O,W,W,O,W,W,O,W,W,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,W,O,O,W,O,O,W,O,W,O,O,O,O,O,O,W,O,O,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,W,O,W,W,O,W,O,W,O,O,W,O,O,O,O,W,O,O,W,W,O,O,O,O,W,O,O,O,O,W,O,O,W,O,W,O,W,O,O,O,O,W,O,O,O,O,O,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,W,O,
        W,W,W,W,O,W,W,W,O,O,W,O,O,O,O,W,O,O,W,O,W,W,W,O,O,W,W,W,O,O,W,O,W,W,O,W,W,W,W,O,O,W,O,O,O,O,O,W,O,W,W,O,O,O,W,O,O,O,O,W,O,W,O,W,O,W,O,W,W,O,W,O,O,W,O,W,W,W,O,O,W,O,O,W,O,O,W,W,W,O,O,O,W,W,O,O,O,O,W,O,O,O,W,O,O,W,O,W,O,O,W,O,W,O,W,O,W,O,O,W,W,O,O,W,W,W,O,O,O,W,W,O,O,W,W,W,W,O,W,W,W,O,O,W,O,O,O,O,W,O,O,W,O,W,W,W,O,O,W,W,W,O,O,W,O,W,W,O,W,W,W,W,O,O,W,O,O,O,O,O,W,O,W,W,O,O,O,W,O,O,O,O,W,O,W,O,W,O,W,O,W,W,O,W,O,O,W,O,W,W,W,O,O,W,O,O,W,O,O,W,W,W,O,O,O,W,W,O,O,O,O,W,O,O,O,W,O,O,W,O,W,O,O,W,O,W,O,W,O,W,O,O,W,W,O,O,W,W,W,O,O,O,W,W,O,O,W,O,W,O,O,O,W,O,O,O,W,W,O,O,W,O,O,W,O,W,W,W,O,O,W,W,W,O,O,O,O,W,O,O,O,W,W,O,O,O,W,W,W,O,W,W,O,W,O,
        W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,W,O,O,O,O,W,O,O,O,O,W,O,O,W,O,W,O,O,W,O,O,W,O,O,W,O,O,W,O,W,O,W,O,O,W,O,O,O,O,W,O,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,O,W,O,W,O,O,O,O,O,W,O,O,O,W,O,O,O,W,O,O,W,O,W,O,W,O,O,W,O,W,O,W,O,W,O,W,W,O,O,O,W,O,O,W,O,O,O,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,W,O,O,O,O,W,O,O,O,O,W,O,O,W,O,W,O,O,W,O,O,W,O,O,W,O,O,W,O,W,O,W,O,O,W,O,O,O,O,W,O,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,O,W,O,W,O,O,O,O,O,W,O,O,O,W,O,O,O,W,O,O,W,O,W,O,W,O,O,W,O,W,O,W,O,W,O,W,W,O,O,O,W,O,O,W,O,O,O,O,O,O,W,O,O,W,O,O,O,O,O,O,W,O,O,W,W,W,O,O,O,O,W,O,W,O,O,W,O,O,W,O,O,O,W,O,O,W,O,O,O,O,W,O,W,O,W,W,O,
        W,O,O,W,O,W,W,W,O,O,O,W,W,W,O,W,W,W,O,O,W,W,W,W,O,W,O,O,O,O,O,W,W,O,O,W,O,O,W,O,W,W,W,O,O,W,W,O,O,W,O,O,W,O,W,W,W,W,O,W,O,O,O,W,O,W,O,O,W,O,O,W,W,O,O,W,O,O,O,O,O,W,W,W,W,O,W,O,O,W,O,W,W,W,O,O,O,O,W,O,O,O,O,W,W,O,O,O,W,O,O,O,O,W,O,W,O,O,W,O,O,W,O,O,W,O,O,O,W,W,W,W,O,W,O,O,W,O,W,W,W,O,O,O,W,W,W,O,W,W,W,O,O,W,W,W,W,O,W,O,O,O,O,O,W,W,O,O,W,O,O,W,O,W,W,W,O,O,W,W,O,O,W,O,O,W,O,W,W,W,W,O,W,O,O,O,W,O,W,O,O,W,O,O,W,W,O,O,W,O,O,O,O,O,W,W,W,W,O,W,O,O,W,O,W,W,W,O,O,O,O,W,O,O,O,O,W,W,O,O,O,W,O,O,O,O,W,O,W,O,O,W,O,O,W,O,O,W,O,O,O,W,W,W,W,O,O,O,W,O,W,W,W,W,O,W,W,W,O,O,O,O,O,W,O,W,W,W,O,O,O,W,W,O,O,O,W,O,O,O,O,W,W,O,O,O,W,W,O,O,O,W,W,O,O
    }
};

Array3D<signed char> GraphicsUtil::charactersAtlasPixelsGrid3D{
    315, 5, 1,
    false, true, true,
    new signed char[315 * 5 * 1] {
        O,W,W,O,O,W,W,W,O,O,O,W,W,W,O,W,W,W,O,O,W,W,W,W,O,W,W,W,W,O,O,W,W,O,O,W,O,O,W,O,W,W,W,O,O,O,W,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,O,W,O,W,O,O,W,O,O,W,W,O,O,W,W,W,O,O,O,W,W,O,O,O,W,W,W,O,O,O,W,W,W,O,W,W,W,W,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,W,W,W,O,O,W,W,O,O,W,W,W,O,O,O,W,W,W,O,W,W,W,O,O,W,W,W,W,O,W,W,W,W,O,O,W,W,O,O,W,O,O,W,O,W,W,W,O,O,O,W,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,O,W,O,W,O,O,W,O,O,W,W,O,O,W,W,W,O,O,O,W,W,O,O,O,W,W,W,O,O,O,W,W,W,O,W,W,W,W,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,W,W,W,O,O,O,W,O,W,W,W,O,O,W,W,W,O,O,O,O,W,W,O,O,W,W,W,O,O,W,W,O,O,W,W,W,W,O,O,W,W,O,O,O,W,W,O,O,O,W,W,O,O,
        W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,W,O,O,O,O,W,O,O,O,O,W,O,O,O,O,W,O,O,W,O,O,W,O,O,O,O,O,W,O,W,O,W,O,O,W,O,O,O,O,W,W,O,W,W,O,W,W,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,W,O,O,W,O,O,W,O,W,O,O,O,O,O,O,W,O,O,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,W,O,W,W,O,W,O,W,O,O,W,O,O,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,W,O,O,O,O,W,O,O,O,O,W,O,O,O,O,W,O,O,W,O,O,W,O,O,O,O,O,W,O,W,O,W,O,O,W,O,O,O,O,W,W,O,W,W,O,W,W,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,W,O,O,W,O,O,W,O,W,O,O,O,O,O,O,W,O,O,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,W,O,W,W,O,W,O,W,O,O,W,O,O,O,O,W,O,O,W,W,O,O,O,O,W,O,O,O,O,W,O,O,W,O,W,O,W,O,O,O,O,W,O,O,O,O,O,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,W,O,
        W,W,W,W,O,W,W,W,O,O,W,O,O,O,O,W,O,O,W,O,W,W,W,O,O,W,W,W,O,O,W,O,W,W,O,W,W,W,W,O,O,W,O,O,O,O,O,W,O,W,W,O,O,O,W,O,O,O,O,W,O,W,O,W,O,W,O,W,W,O,W,O,O,W,O,W,W,W,O,O,W,O,O,W,O,O,W,W,W,O,O,O,W,W,O,O,O,O,W,O,O,O,W,O,O,W,O,W,O,O,W,O,W,O,W,O,W,O,O,W,W,O,O,W,W,W,O,O,O,W,W,O,O,W,W,W,W,O,W,W,W,O,O,W,O,O,O,O,W,O,O,W,O,W,W,W,O,O,W,W,W,O,O,W,O,W,W,O,W,W,W,W,O,O,W,O,O,O,O,O,W,O,W,W,O,O,O,W,O,O,O,O,W,O,W,O,W,O,W,O,W,W,O,W,O,O,W,O,W,W,W,O,O,W,O,O,W,O,O,W,W,W,O,O,O,W,W,O,O,O,O,W,O,O,O,W,O,O,W,O,W,O,O,W,O,W,O,W,O,W,O,O,W,W,O,O,W,W,W,O,O,O,W,W,O,O,W,O,W,O,O,O,W,O,O,O,W,W,O,O,W,O,O,W,O,W,W,W,O,O,W,W,W,O,O,O,O,W,O,O,O,W,W,O,O,O,W,W,W,O,W,W,O,W,O,
        W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,W,O,O,O,O,W,O,O,O,O,W,O,O,W,O,W,O,O,W,O,O,W,O,O,W,O,O,W,O,W,O,W,O,O,W,O,O,O,O,W,O,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,O,W,O,W,O,O,O,O,O,W,O,O,O,W,O,O,O,W,O,O,W,O,W,O,W,O,O,W,O,W,O,W,O,W,O,W,W,O,O,O,W,O,O,W,O,O,O,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,W,O,O,O,O,W,O,O,O,O,W,O,O,W,O,W,O,O,W,O,O,W,O,O,W,O,O,W,O,W,O,W,O,O,W,O,O,O,O,W,O,O,O,W,O,W,O,O,W,O,W,O,O,W,O,W,O,O,O,O,W,O,O,W,O,O,W,O,W,O,O,O,O,O,W,O,O,O,W,O,O,O,W,O,O,W,O,W,O,W,O,O,W,O,W,O,W,O,W,O,W,W,O,O,O,W,O,O,W,O,O,O,O,O,O,W,O,O,W,O,O,O,O,O,O,W,O,O,W,W,W,O,O,O,O,W,O,W,O,O,W,O,O,W,O,O,O,W,O,O,W,O,O,O,O,W,O,W,O,W,W,O,
        W,O,O,W,O,W,W,W,O,O,O,W,W,W,O,W,W,W,O,O,W,W,W,W,O,W,O,O,O,O,O,W,W,O,O,W,O,O,W,O,W,W,W,O,O,W,W,O,O,W,O,O,W,O,W,W,W,W,O,W,O,O,O,W,O,W,O,O,W,O,O,W,W,O,O,W,O,O,O,O,O,W,W,W,W,O,W,O,O,W,O,W,W,W,O,O,O,O,W,O,O,O,O,W,W,O,O,O,W,O,O,O,O,W,O,W,O,O,W,O,O,W,O,O,W,O,O,O,W,W,W,W,O,W,O,O,W,O,W,W,W,O,O,O,W,W,W,O,W,W,W,O,O,W,W,W,W,O,W,O,O,O,O,O,W,W,O,O,W,O,O,W,O,W,W,W,O,O,W,W,O,O,W,O,O,W,O,W,W,W,W,O,W,O,O,O,W,O,W,O,O,W,O,O,W,W,O,O,W,O,O,O,O,O,W,W,W,W,O,W,O,O,W,O,W,W,W,O,O,O,O,W,O,O,O,O,W,W,O,O,O,W,O,O,O,O,W,O,W,O,O,W,O,O,W,O,O,W,O,O,O,W,W,W,W,O,O,O,W,O,W,W,W,W,O,W,W,W,O,O,O,O,O,W,O,W,W,W,O,O,O,W,W,O,O,O,W,O,O,O,O,W,W,O,O,O,W,W,O,O,O,W,W,O,O
    }
};

Map1D<char, int> GraphicsUtil::characterAtlasOffsetsX{};
Map1D<char, int> GraphicsUtil::characterPixelsWidth{};

void GraphicsUtil::InitCharactersAtlas() {
    if (characterAtlasOffsetsX.count() > 0) {
        throw LogicException(LOC);
    }

    characterAtlasOffsetsX.reserve(26/*alpha*/ + 10/*numeric*/);

    int index = 0;
    PushCharInfo('A', 4, &index);
    PushCharInfo('B', 4, &index);
    PushCharInfo('C', 4, &index);
    PushCharInfo('D', 4, &index);
    PushCharInfo('E', 4, &index);
    PushCharInfo('F', 4, &index);
    PushCharInfo('G', 4, &index);
    PushCharInfo('H', 4, &index);
    PushCharInfo('I', 3, &index);
    PushCharInfo('J', 4, &index);
    PushCharInfo('K', 4, &index);
    PushCharInfo('L', 4, &index);
    PushCharInfo('M', 5, &index);
    PushCharInfo('N', 4, &index);
    PushCharInfo('O', 4, &index);
    PushCharInfo('P', 4, &index);
    PushCharInfo('Q', 5, &index);
    PushCharInfo('R', 4, &index);
    PushCharInfo('S', 4, &index);
    PushCharInfo('T', 5, &index);
    PushCharInfo('U', 4, &index);
    PushCharInfo('V', 4, &index);
    PushCharInfo('W', 5, &index);
    PushCharInfo('X', 4, &index);
    PushCharInfo('Y', 4, &index);
    PushCharInfo('Z', 4, &index);

    PushCharInfo('a', 4, &index);
    PushCharInfo('b', 4, &index);
    PushCharInfo('c', 4, &index);
    PushCharInfo('d', 4, &index);
    PushCharInfo('e', 4, &index);
    PushCharInfo('f', 4, &index);
    PushCharInfo('g', 4, &index);
    PushCharInfo('h', 4, &index);
    PushCharInfo('i', 3, &index);
    PushCharInfo('j', 4, &index);
    PushCharInfo('k', 4, &index);
    PushCharInfo('l', 4, &index);
    PushCharInfo('m', 5, &index);
    PushCharInfo('n', 4, &index);
    PushCharInfo('o', 4, &index);
    PushCharInfo('p', 4, &index);
    PushCharInfo('q', 5, &index);
    PushCharInfo('r', 4, &index);
    PushCharInfo('s', 4, &index);
    PushCharInfo('t', 5, &index);
    PushCharInfo('u', 4, &index);
    PushCharInfo('v', 4, &index);
    PushCharInfo('w', 5, &index);
    PushCharInfo('x', 4, &index);
    PushCharInfo('y', 4, &index);
    PushCharInfo('z', 4, &index);

    PushCharInfo('1', 3, &index);
    PushCharInfo('2', 4, &index);
    PushCharInfo('3', 4, &index);
    PushCharInfo('4', 4, &index);
    PushCharInfo('5', 4, &index);
    PushCharInfo('6', 4, &index);
    PushCharInfo('7', 4, &index);
    PushCharInfo('8', 4, &index);
    PushCharInfo('9', 4, &index);
    PushCharInfo('0', 4, &index);

    if (index != 315) {
        throw LogicException(LOC);
    }
}

inline void GraphicsUtil::PushCharInfo(char ch, int charWidth, int* oIndex) {
    characterAtlasOffsetsX.putDirectB(ch, *oIndex);
    characterPixelsWidth.putDirectB(ch, charWidth);

    *oIndex += charWidth + 1;
}

Shader* GraphicsUtil::CreateFilesSourcesShader(std::string vsPath, std::string fsPath) {
    std::string defaultFileVSSource;
    std::string defaultFileFSSource;

    std::string* sourceData;
    std::shared_ptr<base::IFileHandlerBlocking> fh = base::Util_FileSystem::fs->getFileHandlerBlocking(vsPath, base::Util_FileSystem::FSTarget::EmbeddedAssets);
    try {
        sourceData = fh->readCppStringP();
    } catch (Exception& x) {
        throw LogicException(LOC);
    }
    if (sourceData == nullptr) {
        throw LogicException(LOC);
    }
    defaultFileVSSource.assign(sourceData->c_str(), sourceData->size());
    delete sourceData;

    fh = base::Util_FileSystem::fs->getFileHandlerBlocking(fsPath, base::Util_FileSystem::FSTarget::EmbeddedAssets);
    try {
        sourceData = fh->readCppStringP();
    } catch (Exception& x) {
        throw LogicException(LOC);
    }
    if (sourceData == nullptr) {
        throw LogicException(LOC);
    }
    defaultFileFSSource.assign(sourceData->c_str(), sourceData->size());
    delete sourceData;

    Shader* shader = new Shader();
    shader->init(nullptr, nullptr, nullptr, defaultFileVSSource, defaultFileFSSource);

    return shader;
}

std::shared_ptr<VertexAttributesList> GraphicsUtil::VertexAttributesList_2DGeneric = nullptr;
std::shared_ptr<VertexAttributesList> GraphicsUtil::VertexAttributesList_2DGenericTexture = nullptr;
std::shared_ptr<VertexAttributesList> GraphicsUtil::VertexAttributesList_3DGenericTexture = nullptr;
std::shared_ptr<VertexAttributesList> GraphicsUtil::VertexAttributesList_3DGenericColor = nullptr;

std::shared_ptr<graphics::Material> GraphicsUtil::material_2DGenericTexture = nullptr;
std::shared_ptr<graphics::Material> GraphicsUtil::material_3DGenericTexture_solid = nullptr;
//std::shared_ptr<graphics::Material> GraphicsUtil::material_3DGenericTexture_blended = nullptr;

unsigned short* GraphicsUtil::genericIndices = nullptr;
int GraphicsUtil::genericIndices_quadsCount = 0;
int GraphicsUtil::genericIndices_count = 0;

bool GraphicsUtil::IsStaticsInit = base::StaticsInit::AddCbGeneral<bool>(0, []() {
    InitCharactersAtlas();

    VertexAttributesList_2DGeneric = std::make_shared<VertexAttributesList>();
    VertexAttributesList_2DGeneric->pushAttribute(graphics::VertexAttributesList::Type::Position2D);
    VertexAttributesList_2DGeneric->pushAttribute(graphics::VertexAttributesList::Type::DiffuseColor);
    VertexAttributesList_2DGeneric->lock();
    
    VertexAttributesList_2DGenericTexture = std::make_shared<VertexAttributesList>();
    VertexAttributesList_2DGenericTexture->pushAttribute(graphics::VertexAttributesList::Type::Position2D);
    VertexAttributesList_2DGenericTexture->pushAttribute(graphics::VertexAttributesList::Type::DiffuseColor);
    VertexAttributesList_2DGenericTexture->pushAttribute(graphics::VertexAttributesList::Type::TexCoord0);
    //asdB;// refactor to use a ushort and 2 bytes of padding;
    VertexAttributesList_2DGenericTexture->pushAttribute(graphics::VertexAttributesList::Type::DiffuseTextureIndex);
    VertexAttributesList_2DGenericTexture->lock();
    
    VertexAttributesList_3DGenericTexture = std::make_shared<VertexAttributesList>();
    VertexAttributesList_3DGenericTexture->pushAttribute(graphics::VertexAttributesList::Type::Position3D);
    VertexAttributesList_3DGenericTexture->pushAttribute(graphics::VertexAttributesList::Type::Normal3D);
    VertexAttributesList_3DGenericTexture->pushAttribute(graphics::VertexAttributesList::Type::DiffuseColor);
    VertexAttributesList_3DGenericTexture->pushAttribute(graphics::VertexAttributesList::Type::ModelAndNormalTransformIndex);
    VertexAttributesList_3DGenericTexture->pushAttribute(graphics::VertexAttributesList::Type::TexCoord0);
    //asdB;// refactor to use a ushort and 2 bytes of padding;
    VertexAttributesList_3DGenericTexture->pushAttribute(graphics::VertexAttributesList::Type::DiffuseTextureIndex);
    VertexAttributesList_3DGenericTexture->lock();

    VertexAttributesList_3DGenericColor = std::make_shared<VertexAttributesList>();
    VertexAttributesList_3DGenericColor->pushAttribute(graphics::VertexAttributesList::Type::Position3D);
    VertexAttributesList_3DGenericColor->pushAttribute(graphics::VertexAttributesList::Type::Normal3D);
    VertexAttributesList_3DGenericColor->pushAttribute(graphics::VertexAttributesList::Type::DiffuseColor);
    VertexAttributesList_3DGenericColor->pushAttribute(graphics::VertexAttributesList::Type::ModelAndNormalTransformIndex);
	//vertexAttributesList->pushUByte(new std::string("a_padding1"), 3, false);
    VertexAttributesList_3DGenericColor->lock();

    {
        material_2DGenericTexture = std::make_shared<graphics::Material>();
        material_2DGenericTexture->set(new graphics::MABlending(true, IGL::SRC_ALPHA, IGL::ONE_MINUS_SRC_ALPHA));
        material_2DGenericTexture->set(new graphics::MACastColor());
        graphics::MADiffuseTexturesArray* maTexArray = new graphics::MADiffuseTexturesArray();
        // Note. this is set by app when the texture array is created.
        //maTexArray->texturesArray = mainTextureArray0;
        material_2DGenericTexture->set(maTexArray);
    }
    {
        material_3DGenericTexture_solid = std::make_shared<graphics::Material>();
        material_3DGenericTexture_solid->set(graphics::MAInt::NewFrontFace(IGL::CCW));
        material_3DGenericTexture_solid->set(graphics::MAInt::NewCullFace(IGL::BACK));
        material_3DGenericTexture_solid->set(new graphics::MACastColor());
        material_3DGenericTexture_solid->set(new graphics::MACastShadow());
        material_3DGenericTexture_solid->set(new graphics::MAReceiveShadow());
        material_3DGenericTexture_solid->set(new graphics::MADepthTest(true/*isDepthTestEnabled*/, IGL::LESS/*depthFunction*/));
        graphics::MADiffuseTexturesArray* maTexArray = new graphics::MADiffuseTexturesArray();
        // Note. this is set by app when the texture array is created.
        //maTexArray->texturesArray = mainTextureArray0;
        material_3DGenericTexture_solid->set(maTexArray);
    }
    {
        //material_3DGenericTexture_blended = std::make_shared<graphics::Material>();
        //material_3DGenericTexture_blended->set(graphics::MAInt::NewFrontFace(IGL::CCW));
        //material_3DGenericTexture_blended->set(graphics::MAInt::NewCullFace(IGL::BACK));
        //material_3DGenericTexture_blended->set(new graphics::MABlending(true, IGL::SRC_ALPHA, IGL::ONE_MINUS_SRC_ALPHA));
        //material_3DGenericTexture_blended->set(new graphics::MACastColor());
        //material_3DGenericTexture_blended->set(new graphics::MACastShadow());
        //material_3DGenericTexture_blended->set(new graphics::MAReceiveShadow());
        //material_3DGenericTexture_blended->set(new graphics::MADepthTest(true/*isDepthTestEnabled*/, IGL::LESS/*depthFunction*/));
        //graphics::MADiffuseTexturesArray* maTexArray = new graphics::MADiffuseTexturesArray();
        //// Note. this is set by app when the texture array is created.
        ////maTexArray->texturesArray = mainTextureArray0;
        //material_3DGenericTexture_blended->set(maTexArray);
    }

    genericIndices_quadsCount = 1500;
    genericIndices_count = genericIndices_quadsCount * 6;
    genericIndices = new unsigned short[genericIndices_count];
    unsigned int indexOffset = 0;
    unsigned int vertexOffset = 0;
    for (int i = 0; i < genericIndices_quadsCount; i++) {
        genericIndices[indexOffset + 0] = vertexOffset + 0;
        genericIndices[indexOffset + 1] = vertexOffset + 1;
        genericIndices[indexOffset + 2] = vertexOffset + 2;

        genericIndices[indexOffset + 3] = vertexOffset + 0;
        genericIndices[indexOffset + 4] = vertexOffset + 2;
        genericIndices[indexOffset + 5] = vertexOffset + 3;

        indexOffset += 6;
        vertexOffset += 4;
    }
});

GraphicsUtil::GeneratedGeometry2D GraphicsUtil::GenerateTextGeometry2D(std::string text, float geometryWidth, Vector2 anchor, Vector2 pos, Color col) {
    // Generate a voxels grid 2d.
    // Enumerate each character to compute the final grid size.
    Vector2Int gridSize{};
    graphics::GraphicsUtil::GenerateTextVoxelsGrid2D(text, gridSize);

    const signed char O = -1;
    const signed char W = 0;

    Array2D<signed char> textVoxelsGrid2D{ gridSize.x, gridSize.y };
    textVoxelsGrid2D.memSetData(-1);

    // Enumerate the characters again and generate the grid.
    graphics::GraphicsUtil::GenerateTextVoxelsGrid2D(text, gridSize, &textVoxelsGrid2D);

    // Triangulate the above grid to vertices and indices.
    sp<Array1D<ITriangles2DDrawable::LocalVertex>> verticesList = nullptr;
    sp<Array1D<unsigned short>> indicesManaged = nullptr;

    Array2D<signed char> distancesGridX{ textVoxelsGrid2D.getLengthA(), textVoxelsGrid2D.getLengthB() };
    bool isTriangulationRequired = BuildTriangulationDistancesGrid2D(textVoxelsGrid2D, distancesGridX);
    if (isTriangulationRequired) {
        //asd_x;// optimise this probably to not run 2 times and maybe use the new triangles mesh code when implemented.
        int verticesCount = 0;
        int indicesCount = 0;
        // Use these bound because they initialise nicely with any point. Using the int bounds works fine for this case.
        Bounds2D trianglesBounds{ (float)INT_MAX/*xMin*/, (float)INT_MAX/*yMin*/, (float)INT_MIN/*xMax*/, (float)INT_MIN/*yMax*/ };
        Vector2 posOffset{ 0.0f, 0.0f };
        float scale = 1.0f;

        TriangulateDistancesGrid2D(
            distancesGridX, 
            verticesCount, indicesCount, trianglesBounds,
            posOffset, scale, col,
            nullptr/*verticesList*/, nullptr/*indicesManaged*/);

        scale = geometryWidth / trianglesBounds.getWidth();
        Vector2 bakedPosOffset{pos.x - (trianglesBounds.getWidth() * scale) * anchor.x, pos.y - (trianglesBounds.getHeight() * scale) * anchor.y };
        trianglesBounds.set((float)INT_MAX/*xMin*/, (float)INT_MAX/*yMin*/, (float)INT_MIN/*xMax*/, (float)INT_MIN/*yMax*/ );
        bool isTriangulationRequired = BuildTriangulationDistancesGrid2D(textVoxelsGrid2D, distancesGridX);
        verticesList = new Array1D<ITriangles2DDrawable::LocalVertex>(verticesCount);
        indicesManaged = new Array1D<unsigned short>(indicesCount);
        verticesCount = 0;
        indicesCount = 0;
        // Create quads from distancesGrid and convert them to triangles.
        TriangulateDistancesGrid2D(
            distancesGridX, 
            verticesCount, indicesCount, trianglesBounds,
            bakedPosOffset, scale, col,
            verticesList.get()/*verticesList*/, indicesManaged.get()/*indicesManaged*/);
    }

    return GraphicsUtil::GeneratedGeometry2D{ verticesList, indicesManaged };
}

void GraphicsUtil::GenerateTextVoxelsGrid2D(std::string& text, Vector2Int& prevGridSize, Array2D<signed char>* padVoxelGrid) {
    // This must be reset before calling the below logic.
    Vector2Int newGridSize(0, 0);

    bool isPrevCharNewLne = true;
    int cLinePixelsX = 0;
    for (int i = 0; i < text.length(); i++) {
        char ch = text[i];

        if (ch == '\r' || ch == '\n') {
            // Add the previous line to the width as needed.
            if (cLinePixelsX > 0) {
                newGridSize.x = Math::max(newGridSize.x, cLinePixelsX);
                cLinePixelsX = 0;
            }

            // First '\r' causes a new line. Multiple '\r' after cause a 1 voxel lowering only.
            newGridSize.y += isPrevCharNewLne ? 1 : (5 + 1);

            isPrevCharNewLne = true;
        } else if (ch == ' ') {
            cLinePixelsX += 1;

            isPrevCharNewLne = false;
        } else {
            int charAtlasOffsetX;
            int charPixelsWidth;
            try {
                charAtlasOffsetX = characterAtlasOffsetsX.getDirect(ch);
                charPixelsWidth = characterPixelsWidth.getDirect(ch);
            } catch (NotFoundException& x) {
                throw LogicException(LOC);
            }

            // Add a 1 voxel space between characters.
            cLinePixelsX += isPrevCharNewLne ? 0 : 1;

            if (padVoxelGrid != nullptr) {
                // Draw char.
                Vector2Int destOffset{ cLinePixelsX, prevGridSize.y - newGridSize.y - 5 };
                Vector2Int srcOffset{ charAtlasOffsetX, 0 };
                Vector2Int copyCount{ charPixelsWidth, 5 };
                padVoxelGrid->set(
                    destOffset,
                    &charactersAtlasPixelsGrid2D, srcOffset,
                    copyCount
                );
            }

            cLinePixelsX += charPixelsWidth;

            isPrevCharNewLne = false;
        }
    }

    if (cLinePixelsX > 0) {
        // Add the previous line to the width as needed because it wasn't added.
        newGridSize.x = Math::max(newGridSize.x, cLinePixelsX);
        cLinePixelsX = 0;
        (void)cLinePixelsX;

        // Add the last line to the size_y.
        newGridSize.y += 5;
    }

    prevGridSize.set(newGridSize);
}

void GraphicsUtil::GenerateTextVoxelsGrid3D(std::string& text, Vector3Int& prevGridSize, Array3D<signed char>* padVoxelGrid) {
    // This must be reset before calling the below logic.
    Vector3Int newGridSize(0, 0, 1);

    bool isPrevCharNewLne = true;
    int cLinePixelsX = 0;
    for (int i = 0; i < text.length(); i++) {
        char ch = text[i];

        if (ch == '\r' || ch == '\n') {
            // Add the previous line to the width as needed.
            if (cLinePixelsX > 0) {
                newGridSize.x = Math::max(newGridSize.x, cLinePixelsX);
                cLinePixelsX = 0;
            }

            // First '\r' causes a new line. Multiple '\r' after cause a 1 voxel lowering only.
            newGridSize.y += isPrevCharNewLne ? 1 : (5 + 1);

            isPrevCharNewLne = true;
        } else if (ch == ' ') {
            cLinePixelsX += 1;

            isPrevCharNewLne = false;
        } else {
            int charAtlasOffsetX;
            int charPixelsWidth;
            try {
                charAtlasOffsetX = characterAtlasOffsetsX.getDirect(ch);
                charPixelsWidth = characterPixelsWidth.getDirect(ch);
            } catch (NotFoundException& x) {
                throw LogicException(LOC);
            }

            // Add a 1 voxel space between characters.
            cLinePixelsX += isPrevCharNewLne ? 0 : 1;

            if (padVoxelGrid != nullptr) {
                // Draw char.
                Vector3Int destOffset{ cLinePixelsX, prevGridSize.y - newGridSize.y - 5, 0 };
                Vector3Int srcOffset{ charAtlasOffsetX, 0, 0 };
                Vector3Int copyCount{ charPixelsWidth, 5, 1 };
                padVoxelGrid->set(
                    destOffset,
                    &charactersAtlasPixelsGrid3D, srcOffset,
                    copyCount
                );
            }

            cLinePixelsX += charPixelsWidth;

            isPrevCharNewLne = false;
        }
    }

    if (cLinePixelsX > 0) {
        // Add the previous line to the width as needed because it wasn't added.
        newGridSize.x = Math::max(newGridSize.x, cLinePixelsX);
        cLinePixelsX = 0;
        (void)cLinePixelsX;

        // Add the last line to the size_y.
        newGridSize.y += 5;
    }

    prevGridSize.set(newGridSize);
}

bool GraphicsUtil::BuildTriangulationDistancesGrid2D(Array2D<signed char>& tilingGrid, Array2D<signed char>& oDistancesGridX) {
    bool isTriangulationRequired = false;

    Vector2Int gSize{ oDistancesGridX.getLengthA() , oDistancesGridX.getLengthB() };

    for (int gX = gSize.x - 1; gX >= 0; gX--) {
        for (int gY = gSize.y - 1; gY >= 0; gY--) {
            // If on_edge or tileType==-1; Set to 0 distances as needed on each axis separately.
            // Increment the righ and top values if needed.
            signed char tileTypeIndex = tilingGrid.getDirect(gX, gY);

            if (tileTypeIndex == static_cast<signed char>(-1)) {
                isTriangulationRequired = true;
            }

            if (tileTypeIndex == static_cast<signed char>(-1)/*no_tile_physics*/) {
                oDistancesGridX.setDirect(gX, gY, 0);
            } else if (gX == gSize.x - 1/*on_right_edge_and_physics_floor*/) {
                oDistancesGridX.setDirect(gX, gY, 1);
            } else {
                oDistancesGridX.setDirect(gX, gY, oDistancesGridX.getDirect(gX + 1, gY) + 1);
            }
        }
    }

    return isTriangulationRequired;
}

void GraphicsUtil::TriangulateDistancesGrid2D(
    Array2D<signed char>& distancesGrid,
    int& oVerticesCount, int& oIndicesCount, Bounds2D& oTrianglesBounds,
    Vector2& posOffset, float scale, Color& col,
    Array1D<ITriangles2DDrawable::LocalVertex>* verticesList, Array1D<unsigned short>* indicesList)
{
    Vector2Int gSize{ distancesGrid.getLengthA(), distancesGrid.getLengthB() };

    // Enumerate each row from bottom up.
    for (int gX = 0; gX < gSize.x; gX++) {
        int activeQuadStartGPosY = 0;
        // -1; Grab the dist when the gPos is processed.
        int activeQuadGSizeX = -1;

        for (int gY = 0; gY < gSize.y; gY++) {
            char cGDistRight = distancesGrid.getDirect(gX, gY);

            if (cGDistRight == 0) {
                activeQuadStartGPosY = gY + 1;
                activeQuadGSizeX = -1;

                continue;
            }

            if (activeQuadGSizeX == -1) {
                if (gY != activeQuadStartGPosY) {
                    throw LogicException(LOC);
                }

                activeQuadGSizeX = cGDistRight;
            }

            if (gY + 1 >= gSize.y/*top_reached*/ || distancesGrid.getDirect(gX, gY + 1) != activeQuadGSizeX/*top_gPos_has_different_width*/) {
                if (cGDistRight > 0) {
                    Bounds2DInt quadBounds{
                        gX/*xMin*/, gX + activeQuadGSizeX/*xMax*/,
                        activeQuadStartGPosY/*yMin*/, gY + 1/*yMax*/
                    };

                    float quadPosXMin = posOffset.x + (float)quadBounds.min.x * scale;
                    float quadPosXMax = posOffset.x + (float)quadBounds.max.x * scale;
                    float quadPosYMin = posOffset.y + (float)quadBounds.min.y * scale;
                    float quadPosYMax = posOffset.y + (float)quadBounds.max.y * scale;

                    oTrianglesBounds.unionBounds(quadPosXMin, quadPosYMin, quadPosXMax, quadPosYMax);

                    if (verticesList != nullptr) {
                        // Quad bake required.
                        verticesList->setDirect(oVerticesCount + 0, ITriangles2DDrawable::LocalVertex{ Vector2{quadPosXMin, quadPosYMin}, Vector2{-1, -1}, &col });
                        verticesList->setDirect(oVerticesCount + 1, ITriangles2DDrawable::LocalVertex{ Vector2{quadPosXMax, quadPosYMin}, Vector2{-1, -1}, &col });
                        verticesList->setDirect(oVerticesCount + 2, ITriangles2DDrawable::LocalVertex{ Vector2{quadPosXMax, quadPosYMax}, Vector2{-1, -1}, &col });
                        verticesList->setDirect(oVerticesCount + 3, ITriangles2DDrawable::LocalVertex{ Vector2{quadPosXMin, quadPosYMax}, Vector2{-1, -1}, &col });

                        indicesList->setDirect(oIndicesCount + 0, oVerticesCount + 0);
                        indicesList->setDirect(oIndicesCount + 1, oVerticesCount + 1);
                        indicesList->setDirect(oIndicesCount + 2, oVerticesCount + 2);

                        indicesList->setDirect(oIndicesCount + 3, oVerticesCount + 0);
                        indicesList->setDirect(oIndicesCount + 4, oVerticesCount + 2);
                        indicesList->setDirect(oIndicesCount + 5, oVerticesCount + 3);

                        //GlobalAccessLogHandler::devLogHandler->post_debug("Physics grid shape quad bake(xMin, yMin, lenX, lenY) %i %i %i %i", quadBounds.min.x, quadBounds.min.y, quadBounds.getLenX(), quadBounds.getLenY());
                    }

                    oVerticesCount += 4;
                    oIndicesCount += 6;

                    // Must set the distancesGrid to 0 for the quad gCells;
                    distancesGrid.setRegionDirect(&quadBounds, 0);
                }

                activeQuadStartGPosY = gY + 1;
                activeQuadGSizeX = -1;
            }
        }
    }
}
