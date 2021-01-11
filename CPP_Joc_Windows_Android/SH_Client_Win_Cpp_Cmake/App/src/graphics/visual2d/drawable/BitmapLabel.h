#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/InteractiveDrawable2D.h>
#include <graphics/visual/TextureRegion.h>
#include <graphics/visual/BitmapFont.h>
#include <graphics/visual2d/drawable/util/AlignTypeH.h>
#include <graphics/visual2d/drawable/util/AlignTypeV.h>
#include "IBitmapLabel.h"
#include <graphics/visual/TextureArray.h>
#include <graphics/util/GLTypes.h>
#include <base/list/ListDL.h>
#include <base/MM.h>

namespace base {
    class IFontProvider;
    namespace bitmapLabel {
        class TextLine;
    };
};

namespace base {
class BitmapLabel : public InteractiveDrawable2D, virtual public IBitmapLabel {priv typedef InteractiveDrawable2D super;pub dCtor(BitmapLabel);
    pub class LocalInvalidationFlags : public InvalidationFlags {
        pub static const unsigned int font = InvalidationFlags::LAST_USED_BIT_INDEX + 1;
        pub static const unsigned int glyphs = InvalidationFlags::LAST_USED_BIT_INDEX + 2;

        // These flags are used as a optimisation to avoid the overhead that is from InvalidationFlags::transform,color,alpha.
        pub static const unsigned int glyphs_pos = InvalidationFlags::LAST_USED_BIT_INDEX + 3;
        pub static const unsigned int glyphs_color = InvalidationFlags::LAST_USED_BIT_INDEX + 4;
        pub static const unsigned int glyphs_uv = super::InvalidationFlags::LAST_USED_BIT_INDEX + 5;

        pub static const unsigned int LAST_USED_BIT_INDEX = glyphs_uv;
	};

    priv int missingGlyphCharCode = 0;

    //asd_011;// use these.
    priv std::string fontName = "";
    priv int fontSize = 0;
    priv int fontWeight = 0;
    //asd_01;// this will be removed when dynamic texture_regions and auto font rasteriser are implemented.
    priv std::string charactersRange = "";

    priv base::BitmapFont* font = nullptr;
    prot base::IFontProvider* fontProvider;

    priv int alignmentH = base::AlignTypeH::Left;
    priv int alignmentV = base::AlignTypeV::Top;

    priv int globalPosRounding = GlobalPosRounding::ROUND;

    // <0; Entire content is drawn in a single line or in multiple lines using the new line character ('\n').
    // >=0; Content is word_wrapped (if multiple lines enabled) or truncated with the ellipsis text.
    priv float wrappingWidth = -1.0f;
    priv int tabSpacesCount = 4;
    priv float lineHeightMultiplier = 1.0f;

    // >=0; Fixed number of lines. Ellipsis may be used on the last line if content is truncated using a wrappingWidth or an extra line is required.
    // <0; Unlimited lines.
    priv int linesCountOverride = 1;

    // This is used on the last line if limited_lines_count is used and the characters don't fit.
    priv std::string ellipsis = "...";

    priv std::string text = "";

    priv int quadsCount = 0;
    priv graphics::GLTypes::Quad_2DGenericTexture* quadsList = nullptr;

    priv bool isValid_textLines = false;
    priv ListDL<bitmapLabel::TextLine> textLinesList{LOC};

    // These are required here because the setter for this can be called manually by the user and they still need to be dispatched on the next auto sync.
    priv int renderData_invalidationBitMap_Vertices = 0;
    priv int renderData_invalidationBitMap_Indices = 0;

    pub explicit BitmapLabel(base::IFontProvider* fontProvider);
    prot void createMain() override;

    pub std::string& getFontName() final;
    pub void setFontName(std::string& fontName) final;
    pub void setFontNameDirect(std::string fontName) final;
    
    pub int getFontSize() final;
    pub void setFontSize(int fontSize) final;

    pub int getFontWeight() final;
    pub void setFontWeight(int fontWeight) final;

    pub std::string& getCharactersRange() final;
    pub void setCharactersRange(std::string& charactersRange) final;
    pub void setCharactersRangeDirect(std::string charactersRange) final;

    pub int getAlignmentH();
    pub void setAlignmentH(int alignmentH);

    pub int getAlignmentV();
    pub void setAlignmentV(int alignmentV);

    pub int getGlobalPosRounding();
    pub void setGlobalPosRounding(int globalPosRounding);

    pub float getWrappingWidth();
    pub void setWrappingWidth(float wrappingWidth);
    
    pub float getTabSpacesCount();
    pub void setTabSpacesCount(float tabSpacesCount);

    pub float getLineHeightMultiplier();
    pub void setLineHeightMultiplier(float lineHeightMultiplier);

    pub int getLinesCountOverride();
    pub void setLinesCountOverride(int linesCountOverride);

    pub int getLinesCount();

    pub std::string getEllipsis();
    pub void setEllipsis(std::string ellipsis);

    pub std::string getText() final { return text; }
    pub void setTextReference(std::string& text) final;
    pub void setTextDirect(std::string text) final;

    pub float computeLineHeight() final;

    pub int syncDrawSpecsPre(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) override;
    pub int syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) override;

    pub void setSize(float width, float height) override;
    pub void setHeight(float height) override;

    pub std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) override;
    pub void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

	// This computes the glyphs and updates the size.
    pub void validateGlyphsAndMetricsIfNeeded();


    prot void onRenderDataHooked() override;

    priv int sync_font(bool clearInvalidationFlagIfSet);
    priv int sync_vertices_glyphs(bool clearInvalidationFlagIfSet);
    priv int sync_vertices_transform();
    priv int sync_glyphs_uv();
    priv int sync_vertices_color();

    priv void writeLine(std::string& charsList, int& cCharIndex, bitmapLabel::TextLine& line, graphics::TextureArray* newTextureArray);

    priv static void alignLines(
        ListDL<bitmapLabel::TextLine>& textLinesList,
        float textAreaWidth, float textAreaHeight, float lineHeight,
        int alignmentH, int alignmentV, int baseline);

    priv static void managedDrawEllipsis(
        bitmapLabel::TextLine* textLine, std::string& ellipsis,
        float wrappingWidth,
        base::BitmapFont* font, int missingGlyphCharCode, graphics::TextureArray*& newTextureArray);
    priv static float computeEllipsisSolidWidth(
        std::string& ellipsis,
        base::BitmapFont* font, int missingGlyphCharCode);
    priv static float computeWidestLineSolidWidth(ListDL<bitmapLabel::TextLine>* textLinesList);
    priv static int computeRenderableCharactersCount(ListDL<bitmapLabel::TextLine>* textLinesList);

    prot void disposeMain() override;
    pub ~BitmapLabel() override;
};
};
