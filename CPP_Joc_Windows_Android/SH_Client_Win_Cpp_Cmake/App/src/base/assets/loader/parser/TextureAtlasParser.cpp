#include "TextureAtlasParser.h"
#include <base/util/StringUtil.h>
#include <base/assets/util/AssetType.h>
#include <base/MM.h>

TextureAtlasParser::TextureAtlasParser(std::shared_ptr<std::string>& atlassBuffer, std::string& assetFolderPath, bool flipY)
    : atlassBuffer(atlassBuffer), assetFolderPath(assetFolderPath), flipY(flipY)
{
    //void
}

int TextureAtlasParser::computeRegionsCount() {
    int regionsCount = 0;
    for (int i = 0; i < pagesList.size(); i++) {
        regionsCount += pagesList.getPointer(i)->regionsList.size();
    }

    return regionsCount;
}

int TextureAtlasParser::parseData() {
    cLineIndex = 0;

    prepareData();

	if (buildData() != 0) {
		return -4;
	}

	return 0;
}

void TextureAtlasParser::prepareData() {
    linesList = StringUtil::splitToLines(*atlassBuffer, true);
}

Texture* TextureAtlasParser::getTextureAssetFromDependencies(std::string& id, ArrayList<std::shared_ptr<AssetLoaderDependency>>* loaderDependenciesList) {
	for (int i = 0; i < loaderDependenciesList->size(); i++) {
        std::shared_ptr<AssetLoaderDependency> dep = loaderDependenciesList->getDirect(i);
		if (dep->assetIdentifier->type == AssetType::Texture && dep->assetIdentifier->id == id) {
			if (!dep->asset) {
				return nullptr;
			}

			return dep->asset->getAsTexture();
		}
	}

	return nullptr;
}

int TextureAtlasParser::buildRegions(Map1D<std::string, TextureRegion*>* oRegionsMap, ArrayList<std::shared_ptr<AssetLoaderDependency>>* loaderDependenciesList) {
	int totalRegionsCount = computeRegionsCount();
	oRegionsMap->reserve(totalRegionsCount);

	for (int i = 0; i < pagesList.size(); i++) {
		Page& pageConfig = *pagesList.getPointer(i);

		Texture* texture = getTextureAssetFromDependencies(pageConfig.texPath, loaderDependenciesList);
		if (!texture) {
			return -1;
		}

		for (int j = 0; j < pageConfig.regionsList.size(); j++) {
			Region& regionConfig = *pageConfig.regionsList.getPointer(j);

			TextureRegion* texReg = new TextureRegion(
				texture,
				regionConfig.x, regionConfig.y,
				regionConfig.width, regionConfig.height,
				true, flipY
			);

			oRegionsMap->putDirect(
                regionConfig.name,
                texReg
            );
		}
	}

	return 0;
}

int TextureAtlasParser::buildData() {
	bool checkAtlasStructure = true;

	int cPageIndex = 0;
	while(true) {
	    if(pagesList.size() != cPageIndex) {
            throw LogicException(LOC);
	    }

		Page& cPage = *pagesList.append_emplace();

		// Skip the current empty line.
        cLineIndex++;

		if (buildPageData(cPage, checkAtlasStructure) != 0) {
			return -2;
		}

		checkAtlasStructure = false;

        if (cLineIndex >= linesList->size()) {
            break;
        }

        cPageIndex++;
	}

	return 0;
}

int TextureAtlasParser::buildPageData(Page& oCPage, bool checkAtlasStructure) {
	if (buildPageData_texture(oCPage, checkAtlasStructure) != 0) {
		return -1;
	}

	// Read the regions configs.
	int regionsCount;
	if (countPageRegions(regionsCount) != 0) {
		return -4;
	}

	oCPage.regionsList.resize(regionsCount);
	if (buildPageData_regions(regionsCount, oCPage.regionsList, checkAtlasStructure) != 0) {
		return -1;
	}

	return 0;
}

int TextureAtlasParser::buildPageData_texture(Page& oCPage, bool checkAtlasStructure) {
    std::shared_ptr<ArrayList<std::string>> tupleElements;

	// Texture file name.
	std::string texName = linesList->getReference(cLineIndex);
	oCPage.texPath = assetFolderPath + texName;
    cLineIndex++;

	// size
	if ((tupleElements = readTuple(linesList->getReference(cLineIndex))) == nullptr) {
		return -1;
	}
	if (tupleElements->size() != 3) {
		return -2;
	}
	if (checkAtlasStructure && tupleElements->getDirect(0) != "size") {
		return -3;
	}
	oCPage.texWidth = StringUtil::parseInt(tupleElements->getDirect(1));
    oCPage.texHeight = StringUtil::parseInt(tupleElements->getDirect(2));
    cLineIndex++;

	// format
	if ((tupleElements = readTuple(linesList->getReference(cLineIndex))) == nullptr) {
		return -1;
	}
	if (tupleElements->size() != 2) {
		return -2;
	}
	if (checkAtlasStructure && tupleElements->getDirect(0) != "format") {
		return -3;
	}
	oCPage.texFormat = std::string(tupleElements->getDirect(1));
    cLineIndex++;

	// filter
	if ((tupleElements = readTuple(linesList->getReference(cLineIndex))) == nullptr) {
		return -1;
	}
	if (tupleElements->size() != 3) {
		return -2;
	}
	if (checkAtlasStructure && tupleElements->getDirect(0) != "filter") {
		return -3;
	}
	oCPage.texFilter_min = std::string(tupleElements->getDirect(1));
	oCPage.texFilter_mag = std::string(tupleElements->getDirect(2));
	cLineIndex++;

	// repeat
	if ((tupleElements = readTuple(linesList->getReference(cLineIndex))) == nullptr) {
		return -1;
	}
	if (tupleElements->size() != 2) {
		return -2;
	}
	if (checkAtlasStructure && tupleElements->getDirect(0) != "repeat") {
		return -3;
	}
	oCPage.texRepeat = std::string(tupleElements->getDirect(1));
	cLineIndex++;

	return 0;
}

int TextureAtlasParser::countPageRegions(int& oRegionsCount) {
	int regionLinesCount = 0;
	const float linesPerRegion = 7;

	for (int i = cLineIndex; i < linesList->size(); i++) {
		if (linesList->getReference(i).size() == 0) {
			break;
		}

		regionLinesCount++;
	}

	float regionsCountFloat = ((float)regionLinesCount) / linesPerRegion;
	int regionsCount = (int)regionsCountFloat;
	if (regionsCountFloat != regionsCount) {
		// Make sure each region have the same lines count.
		return -1;
	}

	oRegionsCount = regionsCount;

	return 0;
}

int TextureAtlasParser::buildPageData_regions(int regionsCount, ArrayList<Region>& oRregionsList, bool checkAtlasStructure) {
	for (int i = 0; i < regionsCount; i++) {
		Region& cRegion = *oRregionsList.getPointer(i);

		if (parsePageRegion(cRegion, checkAtlasStructure) != 0) {
			return -1;
		}

        checkAtlasStructure = false;
	}

	return 0;
}

int TextureAtlasParser::parsePageRegion(Region& cRegion, bool checkAtlasStructure) {
    std::shared_ptr<ArrayList<std::string>> tupleElements;

	// Texture file name.
	cRegion.name = linesList->getReference(cLineIndex);
	cLineIndex++;

	// rotate
	if ((tupleElements = readTupleChecked(
		linesList->getReference(cLineIndex),
		2, checkAtlasStructure, "rotate")) == nullptr) {
		return -1;
	}
    cRegion.rotate = StringUtil::parseBool(tupleElements->getDirect(1));
	cLineIndex++;

	// xy
	if ((tupleElements = readTupleChecked(
		linesList->getReference(cLineIndex),
		3, checkAtlasStructure, "xy")) == nullptr) {
		return -1;
	}
    cRegion.x = StringUtil::parseInt(tupleElements->getDirect(1));
    cRegion.y = StringUtil::parseInt(tupleElements->getDirect(2));
	cLineIndex++;

	// size
	if ((tupleElements = readTupleChecked(
		linesList->getReference(cLineIndex),
		3, checkAtlasStructure, "size")) == nullptr) {
		return -1;
	}
    cRegion.width = StringUtil::parseInt(tupleElements->getDirect(1));
    cRegion.height = StringUtil::parseInt(tupleElements->getDirect(2));
	cLineIndex++;

	// orig
	if ((tupleElements = readTupleChecked(
		linesList->getReference(cLineIndex),
		3, checkAtlasStructure, "orig")) == nullptr) {
		return -1;
	}
    cRegion.originX = StringUtil::parseInt(tupleElements->getDirect(1));
    cRegion.originY = StringUtil::parseInt(tupleElements->getDirect(2));
	cLineIndex++;

	// offset
	if ((tupleElements = readTupleChecked(
		linesList->getReference(cLineIndex),
		3, checkAtlasStructure, "offset")) == nullptr) {
		return -1;
	}
    cRegion.offsetX = StringUtil::parseFloat(tupleElements->getDirect(1));
    cRegion.offsetY = StringUtil::parseFloat(tupleElements->getDirect(2));
	cLineIndex++;

	// index
	if ((tupleElements = readTupleChecked(
		linesList->getReference(cLineIndex),
		2, checkAtlasStructure, "index")) == nullptr) {
		return -1;
	}
    cRegion.index = StringUtil::parseInt(tupleElements->getDirect(1));
	cLineIndex++;

	return 0;
}

std::shared_ptr<ArrayList<std::string>> TextureAtlasParser::readTuple(std::string& iLine) {
    std::shared_ptr<ArrayList<std::string>> tupleElements = StringUtil::split3(
        iLine,
        " ", ":", ",",
        false
    );

    return tupleElements;
}

std::shared_ptr<ArrayList<std::string>> TextureAtlasParser::readTupleChecked(std::string& iLine, int neededElementsCount, bool checkName, const std::string name) {
    std::shared_ptr<ArrayList<std::string>> tupleElements = StringUtil::split3(
        iLine,
        " ", ":", ",",
        false
    );

	if (tupleElements->size() != neededElementsCount) {
        throw LogicException(LOC);
	}

	if (checkName && tupleElements->getDirect(0) != name) {
        throw LogicException(LOC);
	}

	return tupleElements;
}

TextureAtlasParser::~TextureAtlasParser() {
    //void
}
