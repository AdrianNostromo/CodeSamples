#pragma once

#include <graphics/visual/TextureRegion.h>
#include <base/assets/loader/util/AssetLoaderDependency.h>
#include <base/map/Map1D.h>
#include <base/list/ArrayList.h>

class TextureAtlasParser {
public:
    class Region {
    public:
        std::string name;
        bool rotate;
        int x;
        int y;
        int width;
        int height;
        int originX;
        int originY;
        float offsetX;
        float offsetY;
        int index;
    public:
        Region(Region const&) = delete;
        Region(Region &&) = default;
        Region& operator=(Region const&) = delete;
        Region& operator=(Region &&) = default;
        explicit Region() {
            //void
        }
        ~Region() {
            //void
        }
    };

    class Page {
    public:
        std::string texPath;
        int texWidth;
        int texHeight;
        std::string texFormat;
        std::string texFilter_min;
        std::string texFilter_mag;
        std::string texRepeat;

        ArrayList<Region> regionsList;
    public:
        Page(Page const&) = delete;
        Page(Page &&) = default;
        Page& operator=(Page const&) = delete;
        Page& operator=(Page &&) = default;
        explicit Page() {
            //void
        }

        ~Page() {
            //void
        }
    };
public:
    std::shared_ptr<ArrayList<std::string>> linesList;

    ArrayList<Page> pagesList;

    std::shared_ptr<std::string> atlassBuffer;

    std::string assetFolderPath;
    bool flipY = true;

    int cLineIndex = 0;

public:
    TextureAtlasParser(TextureAtlasParser const&) = delete;
    TextureAtlasParser(TextureAtlasParser &&) = default;
    TextureAtlasParser& operator=(TextureAtlasParser const&) = delete;
    TextureAtlasParser& operator=(TextureAtlasParser &&) = default;

    explicit TextureAtlasParser(std::shared_ptr<std::string>& atlassBuffer, std::string& assetFolderPath, bool flipY);

    int computeRegionsCount();

    int parseData();
    int buildRegions(Map1D<std::string, TextureRegion*>* oRegionsMap, ArrayList<std::shared_ptr<AssetLoaderDependency>>* loaderDependenciesList);

    virtual ~TextureAtlasParser();
private:
    void prepareData();
	Texture* getTextureAssetFromDependencies(std::string& id, ArrayList<std::shared_ptr<AssetLoaderDependency>>* loaderDependenciesList);
	int buildData();
	std::shared_ptr<ArrayList<std::string>> readTuple(std::string& iLine);
	std::shared_ptr<ArrayList<std::string>> readTupleChecked(std::string& iLine, int neededElementsCount, bool checkName, const std::string name);
	int buildPageData(Page& oCPage, bool checkPageStructure);
	int buildPageData_texture(Page& oCPage, bool checkAtlasStructure);
	int countPageRegions(int& oRegionsCount);
	int buildPageData_regions(int regionsCount, ArrayList<Region>& oRegionsList, bool checkAtlasStructure);
	int parsePageRegion(Region& cRegion, bool checkAtlasStructure);
};
