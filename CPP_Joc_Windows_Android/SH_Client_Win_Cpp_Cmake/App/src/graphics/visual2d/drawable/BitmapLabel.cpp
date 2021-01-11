#include "BitmapLabel.h"
#include <base/math/util/BoolUtil.h>
#include <base/exceptions/LogicException.h>
#include <base/math/Math.h>
#include <graphics/model/RenderableLiteInstanceVI.h>
#include <graphics/util/GraphicsUtil.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <base/log/GlobalAccessLogHandler.h>
#include "bitmapLabel/TextLine.h"
#include <base/assets/IFontProvider.h>

using namespace base;
// This namespace is only here because another TextLine class may exist somewhere else.
using namespace base::bitmapLabel;

BitmapLabel::BitmapLabel(base::IFontProvider* fontProvider)
    : super(),
    fontProvider(fontProvider)
{
    // This sets pixel art mode by default.
}

void BitmapLabel::createMain() {
    super::createMain();

    nodePart->verticesUnmanaged = nullptr;
    nodePart->vertices_count = 0;

    nodePart->indicesUnmanaged = graphics::GraphicsUtil::genericIndices;
    nodePart->indices_count = 0;
}

std::string& BitmapLabel::getFontName() {
    return fontName;
}

void BitmapLabel::setFontName(std::string& fontName) {
    if (this->fontName == fontName) {
        return;
    }

    this->fontName = fontName;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::font));
}

void BitmapLabel::setFontNameDirect(std::string fontName) {
    setFontName(fontName);
}

int BitmapLabel::getFontSize() {
    return fontSize;
}

void BitmapLabel::setFontSize(int fontSize) {
    if (this->fontSize == fontSize) {
        return;
    }

    this->fontSize = fontSize;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::font));
}

int BitmapLabel::getFontWeight() {
    return fontWeight;
}

void BitmapLabel::setFontWeight(int fontWeight) {
    if (this->fontWeight == fontWeight) {
        return;
    }

    this->fontWeight = fontWeight;
    
    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::font));
}

std::string& BitmapLabel::getCharactersRange() {
    return charactersRange;
}

void BitmapLabel::setCharactersRange(std::string& charactersRange) {
    if (this->charactersRange == charactersRange) {
        return;
    }

    this->charactersRange = charactersRange;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::font));
}

void BitmapLabel::setCharactersRangeDirect(std::string charactersRange) {
    setCharactersRange(charactersRange);
}

int BitmapLabel::getAlignmentH() {
    return alignmentH;
}

void BitmapLabel::setAlignmentH(int alignmentH) {
    if(this->alignmentH == alignmentH) {
        return ;
    }

    this->alignmentH = alignmentH;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::glyphs));
}

int BitmapLabel::getAlignmentV() {
    return alignmentV;
}

void BitmapLabel::setAlignmentV(int alignmentV) {
    if(this->alignmentV == alignmentV) {
        return ;
    }

    this->alignmentV = alignmentV;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::glyphs));
}

int BitmapLabel::getGlobalPosRounding() {
    return globalPosRounding;
}

void BitmapLabel::setGlobalPosRounding(int globalPosRounding) {
    if(this->globalPosRounding == globalPosRounding) {
        return ;
    }

    this->globalPosRounding = globalPosRounding;

    // This only affects the existing glyphs transforms.
    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::glyphs_pos));
}

float BitmapLabel::getWrappingWidth() {
    return wrappingWidth;
}

void BitmapLabel::setWrappingWidth(float wrappingWidth) {
    if(this->wrappingWidth == wrappingWidth) {
        return ;
    }

    this->wrappingWidth = wrappingWidth;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
        LocalInvalidationFlags::glyphs
    ));
}

float BitmapLabel::getTabSpacesCount() {
    return tabSpacesCount;
}

void BitmapLabel::setTabSpacesCount(float tabSpacesCount) {
    if(this->tabSpacesCount == tabSpacesCount) {
        return ;
    }

    this->tabSpacesCount = tabSpacesCount;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
        LocalInvalidationFlags::glyphs
    ));
}

float BitmapLabel::getLineHeightMultiplier() {
    return lineHeightMultiplier;
}

void BitmapLabel::setLineHeightMultiplier(float lineHeightMultiplier) {
    if(this->lineHeightMultiplier == lineHeightMultiplier) {
        return ;
    }

    this->lineHeightMultiplier = lineHeightMultiplier;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
        LocalInvalidationFlags::glyphs
    ));
}

int BitmapLabel::getLinesCountOverride() {
    return linesCountOverride;
}

void BitmapLabel::setLinesCountOverride(int linesCountOverride) {
    if(this->linesCountOverride == linesCountOverride) {
        return ;
    }

    this->linesCountOverride = linesCountOverride;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
        LocalInvalidationFlags::glyphs
    ));
}

int BitmapLabel::getLinesCount() {
    if (linesCountOverride >= 0) {
        return linesCountOverride;
    }

    return textLinesList.count();
}

std::string BitmapLabel::getEllipsis() {
    return ellipsis;
}

void BitmapLabel::setEllipsis(std::string ellipsis) {
    if(this->ellipsis == ellipsis) {
        return ;
    }

    this->ellipsis = ellipsis;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
        LocalInvalidationFlags::glyphs
    ));
}

void BitmapLabel::setTextReference(std::string& text) {
    if(this->text == text) {
        return ;
    }

    this->text = text;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::glyphs));
}

void BitmapLabel::setTextDirect(std::string text) {
    if(this->text == text) {
        return ;
    }
    if (text.size() > 10000) {
        // Looks like memory error.
        throw LogicException(LOC);
    }

    this->text = text;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::glyphs));
}

float BitmapLabel::computeLineHeight() {
    float scaledLineHeight = Math::round(font->lineHeight * lineHeightMultiplier);

    return scaledLineHeight;
}

int BitmapLabel::syncDrawSpecsPre(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
	int r = super::syncDrawSpecsPre(combinedInvalidationBitMap, renderer, preRenderData);
	if (r != 0) {
		return r;
	}

    if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap,
        LocalInvalidationFlags::font))
    {
        if (sync_font(false/*clearInvalidationFlagIfSet*/) != 0) {
            return -2;
        }
    }
    if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap,
        LocalInvalidationFlags::glyphs))
    {
        if (sync_vertices_glyphs(false/*clearInvalidationFlagIfSet*/) != 0) {
            return -2;
        }

        if (renderData_invalidationBitMap_Vertices != 0 || renderData_invalidationBitMap_Indices != 0) {
            nodePart->invalidate(renderData_invalidationBitMap_Vertices, renderData_invalidationBitMap_Indices,
                0/*invalidationBitMask_ModelTransforms*/, 0/*invalidationBitMask_ModelNormalTransforms*/
            );
            renderData_invalidationBitMap_Vertices = 0;
            renderData_invalidationBitMap_Indices = 0;
        }
    }

	return 0;
}

int BitmapLabel::syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
	int r = super::syncDrawSpecs(combinedInvalidationBitMap, renderer, preRenderData);
	if (r != 0) {
		return r;
	}

    if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap,
        InvalidationFlags::transform,
        LocalInvalidationFlags::glyphs_pos))
    {
		if (sync_vertices_transform() != 0) {
			return -1;
		}
	}
    if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, LocalInvalidationFlags::glyphs_uv)) {
        // Note. Also update on texture change because uv are not updated if there is no texture.
        if (sync_glyphs_uv() != 0) {
            return -2;
        }
    }
	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap,
		InvalidationFlags::alpha,
		InvalidationFlags::tintColor,
		LocalInvalidationFlags::glyphs_color))
	{
		if (sync_vertices_color() != 0) {
			return -3;
		}
	}

	return 0;
}

int BitmapLabel::sync_font(bool clearInvalidationFlagIfSet) {
    if (!BoolUtil::isBitAtIndex(invalidationBitMap, LocalInvalidationFlags::font)) {
        // Glyphs are valid, no need to validate.

        return 0;
    }

    if (clearInvalidationFlagIfSet) {
        // Manually remove the invalidation flag here. This is used when manually syncing vertices glyphs.

        invalidationBitMap = BoolUtil::clearBitAtIndexDirect(invalidationBitMap, LocalInvalidationFlags::font);
    }

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::glyphs));

    font = fontProvider->getFont(
        fontName, fontSize, fontWeight, charactersRange,
        false/*retNullOnBadParams*/,
        true/*mustExits*/
    );

    return 0;
}

int BitmapLabel::sync_vertices_glyphs(bool clearInvalidationFlagIfSet) {
    if (!BoolUtil::isBitAtIndex(invalidationBitMap, LocalInvalidationFlags::glyphs)) {
        // Glyphs are valid, no need to validate.

        return 0;
    }

    if (clearInvalidationFlagIfSet) {
        // Manually remove the invalidation flag here. This is used when manually syncing vertices glyphs.

        invalidationBitMap = BoolUtil::clearBitAtIndexDirect(invalidationBitMap, LocalInvalidationFlags::glyphs);
    }

    graphics::TextureArray* newTextureArray = nullptr;
    int renderableCharactersCount = 0;
    float widestLineSolidWidth = 0.0f;

    int drawSpecs_invalidationBitMap = 0;

    textLinesList.clear();

    if(font != nullptr) {
        int cCharIndex = 0;
        while (cCharIndex < text.length() && (linesCountOverride < 0/*unlimited*/ || textLinesList.count() < linesCountOverride/*line_capacity_exists*/)) {
            // Create and fill a new line because there are some characters (create a new line even if only space characters exist).
            TextLine* textLine = &textLinesList.appendEmplace(textLinesList.count())->data;
            writeLine(text, cCharIndex, *textLine, newTextureArray);

            if (textLine->charsList.count() == 0) {
                // The glyph was too large to fit in a full line, stop characters writing.
                break;
            }
        }

        if (cCharIndex < text.length() && textLinesList.count() > 0 && ellipsis.size() > 0) {
            // No all characters where written, draw a elipsis on the last line.
            TextLine* textLine = &textLinesList.getLast()->data;

            managedDrawEllipsis(
                &textLinesList.getLast()->data, ellipsis,
                wrappingWidth,
                font, missingGlyphCharCode, newTextureArray);
        }

        widestLineSolidWidth = computeWidestLineSolidWidth(&textLinesList);
        renderableCharactersCount = computeRenderableCharactersCount(&textLinesList);

        float scaledLineHeight = computeLineHeight();
        float textAreaWidth = (wrappingWidth >= 0) ? wrappingWidth : widestLineSolidWidth;
        float textAreaHeight = ((linesCountOverride >= 0) ? linesCountOverride : textLinesList.count()) * scaledLineHeight;
        alignLines(
            textLinesList,
            textAreaWidth, textAreaHeight, scaledLineHeight,
            alignmentH, alignmentV, font->baseline
        );
    }

    // Build quads list.
    if(renderableCharactersCount != quadsCount) {
        if (quadsList != nullptr) {
            delete[] quadsList;

            quadsList = nullptr;
        }

        quadsCount = renderableCharactersCount;
        if(quadsCount > 0) {
            quadsList = new graphics::GLTypes::Quad_2DGenericTexture[quadsCount];
        }
        nodePart->verticesUnmanaged = quadsList;
        nodePart->vertices_count = quadsCount * 4;
        if (quadsCount > graphics::GraphicsUtil::genericIndices_quadsCount) {
            // The pre-cached indices data from GraphicsUtil has a quads limit that got reached by the text. Increase that list if needed.
            // This should not occur under normal app ussage.
            // Clip the text indices and dispatch a warning.
            nodePart->indices_count = graphics::GraphicsUtil::genericIndices_quadsCount * 6;
            GlobalAccessLogHandler::devLogHandler->post_debug("BitmapLabel tried to use more cached_indices that available, clipping indices to the max amount.");
        } else {
            nodePart->indices_count = quadsCount * 6;
        }

        drawSpecs_invalidationBitMap = BoolUtil::setBitAtIndexDirect(drawSpecs_invalidationBitMap,
            LocalInvalidationFlags::glyphs_pos,
            LocalInvalidationFlags::glyphs_color
        );

        renderData_invalidationBitMap_Vertices |= graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup;
        renderData_invalidationBitMap_Indices |= graphics::IGeometryProvider_Indices::InvalidationBitGroups::countAndData_bitGroup;
    }else {
        renderData_invalidationBitMap_Vertices |= graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup;
    }

    int currentQuadIndex = 0;
    for (ListDL<TextLine>::Entry* entry = textLinesList.getFirst(); entry != nullptr; entry = entry->next) {
        TextLine* textLine = &entry->data;

        for (ListDL<TextLine::TextChar>::Entry* entry = textLine->charsList.getFirst(); entry != nullptr; entry = entry->next) {
            TextLine::TextChar& textChar = entry->data;
            
            if(textChar.glyph->textureRegion == nullptr) {
                // This character is not rendered (maybe a space), skip it.
                continue;
            }

            graphics::GLTypes::Quad_2DGenericTexture& quad = quadsList[currentQuadIndex];

            TextureRegion* region = textChar.glyph->textureRegion;

            quad.bl.a_diffuseTextureIndex = region->getTexture()->packedTextureIndex;
            quad.br.a_diffuseTextureIndex = region->getTexture()->packedTextureIndex;
            quad.tr.a_diffuseTextureIndex = region->getTexture()->packedTextureIndex;
            quad.tl.a_diffuseTextureIndex = region->getTexture()->packedTextureIndex;

            currentQuadIndex++;
        }
    }
    if(currentQuadIndex != quadsCount) {
        // More or less quads were created than character to render.
        throw LogicException(LOC);
    }

    float visualSize_w;
    float visualSize_h;
    if(wrappingWidth >= 0) {
        visualSize_w = wrappingWidth;
    }else {
        visualSize_w = widestLineSolidWidth;
    }
    if(linesCountOverride >= 0) {
        visualSize_h = linesCountOverride * font->lineHeight * lineHeightMultiplier;
    }else {
        visualSize_h = textLinesList.count() * font->lineHeight * lineHeightMultiplier;
    }
    // Remove 2 baselines so the font will have a correct anchor_y behaviour.
    /// Note. This should use the small_character_height (x_height font metric) but that is not available. Using a extra baseline here replaces that and works for the used fonts but it is not the correct solution.
    visualSize_h -= font->baseline * 2.0f;

    super::setSize(
        visualSize_w,
        visualSize_h
    );

    // On glyphs change, need to invalidate the glyphs_pos to make sure the vertices are positioned at the correct positions.
    drawSpecs_invalidationBitMap = BoolUtil::setBitAtIndexDirect(drawSpecs_invalidationBitMap,
        LocalInvalidationFlags::glyphs_pos,
        LocalInvalidationFlags::glyphs_uv
    );

    if(drawSpecs_invalidationBitMap != 0) {
        invalidateDrawSpecs(drawSpecs_invalidationBitMap);
    }

	return 0;
}

int BitmapLabel::sync_vertices_transform() {
	Affine2& transform = getRenderingTransformAffine2();

    // Construct quads.
    int currentQuadIndex = 0;
    for (ListDL<TextLine>::Entry* entry = textLinesList.getFirst(); entry != nullptr; entry = entry->next) {
        TextLine* textLine = &entry->data;

        for (ListDL<TextLine::TextChar>::Entry* entry = textLine->charsList.getFirst(); entry != nullptr; entry = entry->next) {
            TextLine::TextChar& textChar = entry->data;

            if(textChar.glyph->textureRegion == nullptr) {
                // This character is not rendered (maybe a space), skip it.
                continue;
            }

            graphics::GLTypes::Quad_2DGenericTexture &quad = quadsList[currentQuadIndex];

            float xMin = textLine->posX + textChar.charOffsetX + textChar.glyph->offsetX;
            float yMin = textLine->posY + textChar.glyph->offsetY;

            float xMax = xMin + textChar.glyph->width;
            float yMax = yMin + textChar.glyph->height;

            // Construct quads.
            // (0.0f, 0.0f)
            Vector2 bl{xMin, yMin};
            transform.applyTo(bl);
            // (1.0f, 0.0f)
            Vector2 br{ xMax, yMin };
            transform.applyTo(br);
            // (1.0f, 1.0f)
            Vector2 tr{ xMax, yMax };
            transform.applyTo(tr);
            // (0.0f, 1.0f)
            Vector2 tl{xMin, yMax};
            transform.applyTo(tl);

            if(globalPosRounding == GlobalPosRounding::NONE) {
                quad.bl.pos.x = bl.x;
                quad.bl.pos.y = bl.y;

                quad.br.pos.x = br.x;
                quad.br.pos.y = br.y;

                quad.tr.pos.x = tr.x;
                quad.tr.pos.y = tr.y;

                quad.tl.pos.x = tl.x;
                quad.tl.pos.y = tl.y;
            }else if(globalPosRounding == GlobalPosRounding::ROUND) {
                quad.bl.pos.x = Math::round(bl.x);
                quad.bl.pos.y = Math::round(bl.y);

                quad.br.pos.x = Math::round(br.x);
                quad.br.pos.y = Math::round(br.y);

                quad.tr.pos.x = Math::round(tr.x);
                quad.tr.pos.y = Math::round(tr.y);

                quad.tl.pos.x = Math::round(tl.x);
                quad.tl.pos.y = Math::round(tl.y);
            }else if(globalPosRounding == GlobalPosRounding::FLOOR) {
                quad.bl.pos.x = Math::floor(bl.x);
                quad.bl.pos.y = Math::floor(bl.y);

                quad.br.pos.x = Math::floor(br.x);
                quad.br.pos.y = Math::floor(br.y);

                quad.tr.pos.x = Math::floor(tr.x);
                quad.tr.pos.y = Math::floor(tr.y);

                quad.tl.pos.x = Math::floor(tl.x);
                quad.tl.pos.y = Math::floor(tl.y);
            }else if(globalPosRounding == GlobalPosRounding::CEIL) {
                quad.bl.pos.x = Math::ceil(bl.x);
                quad.bl.pos.y = Math::ceil(bl.y);

                quad.br.pos.x = Math::ceil(br.x);
                quad.br.pos.y = Math::ceil(br.y);

                quad.tr.pos.x = Math::ceil(tr.x);
                quad.tr.pos.y = Math::ceil(tr.y);

                quad.tl.pos.x = Math::ceil(tl.x);
                quad.tl.pos.y = Math::ceil(tl.y);
            }else {
                throw LogicException(LOC);
            }

            currentQuadIndex++;
        }
    }

	nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);

	return 0;
}

int BitmapLabel::sync_glyphs_uv() {
    int currentQuadIndex = 0;
    for (ListDL<TextLine>::Entry* entry = textLinesList.getFirst(); entry != nullptr; entry = entry->next) {
        TextLine* textLine = &entry->data;

        for (ListDL<TextLine::TextChar>::Entry* entry = textLine->charsList.getFirst(); entry != nullptr; entry = entry->next) {
            TextLine::TextChar& textChar = entry->data;

            if (textChar.glyph->textureRegion == nullptr) {
                // This character is not rendered (maybe a space), skip it.
                continue;
            }

            graphics::GLTypes::Quad_2DGenericTexture& quad = quadsList[currentQuadIndex];

            const TextureRegion* region = textChar.glyph->textureRegion;

            float u = (region != nullptr) ? region->getU() : 0;
            float v = (region != nullptr) ? region->getV() : 0;
            float u2 = (region != nullptr) ? region->getU2() : 0;
            float v2 = (region != nullptr) ? region->getV2() : 0;

            quad.bl.uv.x = u;
            quad.bl.uv.y = v;

            quad.br.uv.x = u2;
            quad.br.uv.y = v;

            quad.tr.uv.x = u2;
            quad.tr.uv.y = v2;

            quad.tl.uv.x = u;
            quad.tl.uv.y = v2;

            currentQuadIndex++;
        }
    }
    if (currentQuadIndex != quadsCount) {
        // More or less quads were created than character to render.
        throw LogicException(LOC);
    }

    nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);

    return 0;
}

int BitmapLabel::sync_vertices_color() {
	Color tCol{};

	tCol.set(getGlobalTintColor());
	tCol.a *= getGlobalAlpha();

	for(int i=0;i<quadsCount;i++) {
        graphics::GLTypes::Quad_2DGenericTexture& quad = quadsList[i];

        quad.bl.diffuseColor.r = static_cast<unsigned char>(tCol.r * 255.0f);
        quad.bl.diffuseColor.g = static_cast<unsigned char>(tCol.g * 255.0f);
        quad.bl.diffuseColor.b = static_cast<unsigned char>(tCol.b * 255.0f);
        quad.bl.diffuseColor.a = static_cast<unsigned char>(tCol.a * 255.0f);

        quad.br.diffuseColor.r = static_cast<unsigned char>(tCol.r * 255.0f);
        quad.br.diffuseColor.g = static_cast<unsigned char>(tCol.g * 255.0f);
        quad.br.diffuseColor.b = static_cast<unsigned char>(tCol.b * 255.0f);
        quad.br.diffuseColor.a = static_cast<unsigned char>(tCol.a * 255.0f);

        quad.tr.diffuseColor.r = static_cast<unsigned char>(tCol.r * 255.0f);
        quad.tr.diffuseColor.g = static_cast<unsigned char>(tCol.g * 255.0f);
        quad.tr.diffuseColor.b = static_cast<unsigned char>(tCol.b * 255.0f);
        quad.tr.diffuseColor.a = static_cast<unsigned char>(tCol.a * 255.0f);

        quad.tl.diffuseColor.r = static_cast<unsigned char>(tCol.r * 255.0f);
        quad.tl.diffuseColor.g = static_cast<unsigned char>(tCol.g * 255.0f);
        quad.tl.diffuseColor.b = static_cast<unsigned char>(tCol.b * 255.0f);
        quad.tl.diffuseColor.a = static_cast<unsigned char>(tCol.a * 255.0f);
    }

    nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);

	return 0;
}

void BitmapLabel::writeLine(std::string& charsList, int& cCharIndex, TextLine& textLine, graphics::TextureArray* newTextureArray) {
    int charsCount = charsList.length();

    // This is used to remove the last word when word wrapping occurs.
    /// Note. Character wrapping is used if there is only 1 word in the line.
    /// Words delimiter is 
    int lastWordStartCharIndex = -1;
    int lastWordInLineCharIndex = -1;
    bool isWordDividerCharActive = true;
    enum CharCategory { WordDivider, WordPart } charCategory = WordDivider;
    while (cCharIndex < charsCount) {
        int ch = charsList[cCharIndex++];

        BitmapFont::Glyph* glyph = nullptr;
        // This is used for tabs to insert ~4 spaces. The combined width is included in the check.
        int glyphsInsertCount = 1;
        bool endCurrentLine = false;

        if (ch == '\n') {
            if (!endCurrentLine && (linesCountOverride < 0/*unlimited*/ || linesCountOverride > 1/*fixed_cont*/)) {
                endCurrentLine = true;
            } else {
                // Skip \n if single line mode.
                continue;
            }
        } else if (ch == '\t') {
            // Tab spaces are inserted on the same line.
            glyph = font->getGlyph(' ');
            if (glyph == nullptr) {
                glyph = font->getGlyph(missingGlyphCharCode);
                if (glyph == nullptr) {
                    throw LogicException(LOC);
                }
            }
            
            glyphsInsertCount = tabSpacesCount;
        } else {
            glyph = font->getGlyph(ch);
            if (glyph == nullptr) {
                glyph = font->getGlyph(missingGlyphCharCode);
                if (glyph == nullptr) {
                    throw LogicException(LOC);
                }
            }
        }

        if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
            // Word divider character.
            isWordDividerCharActive = true;

            charCategory = WordDivider;
        } else {
            charCategory = WordPart;
        }

        if (!endCurrentLine) {
            // End current line if new glyph has a texture and won't fit.
            if (wrappingWidth >= 0/*limited_width*/ && glyph->textureRegion != nullptr/*renderable_character*/
                && textLine.getSolidWidthAndSuffixSpace() + glyph->width * glyphsInsertCount > wrappingWidth/*new_size_is_too_wide*/)
            {
                // Don't insert the character.
                cCharIndex--;

                // Need to do word wrapping splitting.
                /// Word splitting is skipped if the line contains only 1 long word.
                /// Note. The last character was a solid character.
                
                // Use this variable to check if word or character wrapping is done because the check needs to be per line, not per entire_text.
                if (lastWordInLineCharIndex > 0) {
                    // The last word in the line is smaller than the entire line, wrap it completly.
                    // Need to go back with cCharIndex until lastWordStartCharIndex is reached.
                    /// Note. When pop-ing characters, '\t' needs to decrement cCharIndex once but pop tabSpacesCount glyphs from the line.

                    while (cCharIndex > lastWordStartCharIndex) {
                        cCharIndex--;
                        int ch = charsList[cCharIndex];

                        int charsToPosCount = 1;
                        if (ch == '\t') {
                            charsToPosCount = tabSpacesCount;
                        }

                        textLine.popChar(charsToPosCount);
                    }
                }

                endCurrentLine = true;
            }
        }

        if (endCurrentLine) {
            break;
        }

        if (charCategory == WordPart && isWordDividerCharActive) {
            isWordDividerCharActive = false;

            lastWordStartCharIndex = cCharIndex - 1;
            lastWordInLineCharIndex = textLine.charsList.count();
        }

        if (glyph != nullptr && glyph->advanceX > 0) {
            textLine.pushGlyph(glyph, glyphsInsertCount, newTextureArray);
        }
    }
}

void BitmapLabel::alignLines(
    ListDL<TextLine>& textLinesList,
    float textAreaWidth, float textAreaHeight, float lineHeight,
    int alignmentH, int alignmentV, int baseline)
{
    // Handle h alignment positioning. Modify the line posX.
    if(alignmentH == base::AlignTypeH::Left) {
        for (ListDL<TextLine>::Entry* entry = textLinesList.getFirst(); entry != nullptr; entry = entry->next) {
            TextLine* textLine = &entry->data;

            textLine->posX = 0.0f;
        }
    }else if(alignmentH == base::AlignTypeH::Center) {
        for (ListDL<TextLine>::Entry* entry = textLinesList.getFirst(); entry != nullptr; entry = entry->next) {
            TextLine* textLine = &entry->data;

            textLine->posX = Math::round((textAreaWidth - textLine->getSolidWidth()) / 2.0f);
        }
    }else if(alignmentH == base::AlignTypeH::Right) {
        for (ListDL<TextLine>::Entry* entry = textLinesList.getFirst(); entry != nullptr; entry = entry->next) {
            TextLine* textLine = &entry->data;

            textLine->posX = Math::round(textAreaWidth - textLine->getSolidWidth());
        }
    }else {
        throw LogicException(LOC);
    }

    // Handle v alignment positioning. Modify the line posY.
    // Note. All lines are moved down so the line_origin is at the bottom target pos_y;
    float offsetY = -baseline;
    if(alignmentV == base::AlignTypeV::Top) {
        offsetY += textAreaHeight - lineHeight;
    }else if(alignmentV == base::AlignTypeV::Center) {
        offsetY += (textAreaHeight - lineHeight) / 2.0f + ((textLinesList.count() - 1) * lineHeight) / 2.0f;
    }else if(alignmentV == base::AlignTypeV::Bottom) {
        offsetY += (textLinesList.count() - 1) * lineHeight;
    }else {
        throw LogicException(LOC);
    }
    int i = 0;
    for (ListDL<TextLine>::Entry* entry = textLinesList.getFirst(); entry != nullptr; entry = entry->next) {
        TextLine* textLine = &entry->data;

        textLine->posY = Math::round(offsetY - i * lineHeight);

        i++;
    }
}

void BitmapLabel::managedDrawEllipsis(
    TextLine* textLine, std::string& ellipsis,
    float wrappingWidth,
    base::BitmapFont* font, int missingGlyphCharCode, graphics::TextureArray*& newTextureArray)
{
    if(textLine == nullptr || ellipsis.size() <= 0) {
        return ;
    }

    float ellipsisSolidWidth = computeEllipsisSolidWidth(ellipsis, font, missingGlyphCharCode);

    if(wrappingWidth >= 0) {
        // Start removing end characters until the ellipsis will fit. All the end spaces are removed (use the solidWidth and go backwards).
        while(textLine->charsList.count() > 0) {
            if(textLine->getSolidWidth() + ellipsisSolidWidth <= wrappingWidth) {
                // Ellipsis fits.
                break;
            }

            textLine->popChar(1/*charsToPopCount*/);
        }
    }

    // Draw as much of the ellipsis as possible.
    for(int i=0;i<(int)ellipsis.size();i++) {
        int ch = ellipsis[i];

        BitmapFont::Glyph* glyph = font->getGlyph(ch);
        if(glyph == nullptr) {
            glyph = font->getGlyph(missingGlyphCharCode);
            if(glyph == nullptr) {
                throw LogicException(LOC);
            }
        }

        if(wrappingWidth >= 0) {
            if(textLine->getSolidWidth() + textLine->getSuffixSpace() + glyph->width > wrappingWidth) {
                // The glyph will be outside of the line width range, stop drawing ellipsis chars.
                break;
            }
        }

        textLine->pushGlyph(glyph, 1/*glyphsInsertCount*/, newTextureArray);
    }
}

float BitmapLabel::computeEllipsisSolidWidth(
    std::string& ellipsis,
    base::BitmapFont* font, int missingGlyphCharCode)
{
    float solidWidth = 0.0f;
    float suffixSpace = 0.0f;
    for(int i=0;i<(int)ellipsis.size();i++) {
        int ch = ellipsis[i];

        BitmapFont::Glyph* glyph = font->getGlyph(ch);
        if(glyph == nullptr) {
            glyph = font->getGlyph(missingGlyphCharCode);
            if(glyph == nullptr) {
                throw LogicException(LOC);
            }
        }

        if(glyph->textureRegion != nullptr) {
            solidWidth += suffixSpace + glyph->width;
            suffixSpace = glyph->advanceX - glyph->width;
        }else {
            suffixSpace += glyph->advanceX;
        }
    }

    return solidWidth;
}

float BitmapLabel::computeWidestLineSolidWidth(ListDL<TextLine>* textLinesList) {
    float r = 0.0f;

    for (ListDL<TextLine>::Entry* entry = textLinesList->getFirst(); entry != nullptr; entry = entry->next) {
        TextLine* textLine = &entry->data;

        r = Math::max(r, textLine->getSolidWidth());
    }

    return r;
}

int BitmapLabel::computeRenderableCharactersCount(ListDL<TextLine>* textLinesList) {
    int r = 0;
    for (ListDL<TextLine>::Entry* entry = textLinesList->getFirst(); entry != nullptr; entry = entry->next) {
        TextLine* textLine = &entry->data;

        r += textLine->getRenderableCharactersCount();
    }

    return r;
}

void BitmapLabel::onRenderDataHooked() {
	super::onRenderDataHooked();

    nodePart->invalidate(
        graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup, 
        graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup,
        0,
        0
    );
}

void BitmapLabel::setSize(float width, float height) {
    // Not allowed for this visual type. Height value is computed and stuff.
    throw LogicException(LOC);
}

void BitmapLabel::setHeight(float height) {
    // Not allowed for this visual type. Value is computed and stuff.
    throw LogicException(LOC);
}

std::shared_ptr<base::IWrappedValue> BitmapLabel::getPropertyValue(std::string property) {
    if(property == "alignmentH") {
        return base::IWrappedValue::new_int(alignmentH);
    }else if(property == "alignmentV") {
        return base::IWrappedValue::new_int(alignmentV);
    }

    else if(property == "globalPosRounding") {
        return base::IWrappedValue::new_int(globalPosRounding);
    }

    else if(property == "wrappingWidth") {
        return base::IWrappedValue::new_float(wrappingWidth);
    }else if(property == "tabSpacesCount") {
        return base::IWrappedValue::new_int(tabSpacesCount);
    }else if(property == "lineHeightMultiplier") {
        return base::IWrappedValue::new_float(lineHeightMultiplier);
    }

    else if(property == "linesCountOverride") {
        return base::IWrappedValue::new_int(linesCountOverride);
    }

    else if(property == "ellipsis") {
        return base::IWrappedValue::new_String(ellipsis);
    }

    else if(property == "text") {
        return base::IWrappedValue::new_String(text);
    }
    
    else if(property == "fontName") {
        return base::IWrappedValue::new_String(fontName);
    }else if(property == "fontSize") {
        return base::IWrappedValue::new_int(fontSize);
    }else if(property == "fontWeight") {
        return base::IWrappedValue::new_int(fontWeight);
    }else if (property == "charactersRange") {
        return base::IWrappedValue::new_String(charactersRange);
    }

    return super::getPropertyValue(property);
}

void BitmapLabel::setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) {
    if(property == "alignmentH") {
        setAlignmentH(value->getDirectAs_int());
    }else if(property == "alignmentV") {
        setAlignmentV(value->getDirectAs_int());
    }

    else if(property == "globalPosRounding") {
        setGlobalPosRounding(value->getDirectAs_int());
    }

    else if(property == "wrappingWidth") {
        setWrappingWidth(value->getDirectAs_float());
    }else if(property == "tabSpacesCount") {
        setTabSpacesCount(value->getDirectAs_int());
    }else if(property == "lineHeightMultiplier") {
        setLineHeightMultiplier(value->getDirectAs_float());
    }

    else if(property == "linesCountOverride") {
        setLinesCountOverride(value->getDirectAs_int());
    }

    else if(property == "ellipsis") {
        setEllipsis(value->getReferenceAs_String());
    }

    else if(property == "text") {
        setTextReference(value->getReferenceAs_String());
    }
    
    else if(property == "fontName") {
        setFontName(value->getReferenceAs_String());
    }else if(property == "fontSize") {
        setFontSize(value->getDirectAs_int());
    }else if(property == "fontWeight") {
        setFontWeight(value->getDirectAs_int());
    } else if (property == "charactersRange") {
        setCharactersRange(value->getReferenceAs_String());
    }

    else {
        super::setPropertyValue(property, value);
    }
}

void BitmapLabel::validateGlyphsAndMetricsIfNeeded() {
    sync_font(true/*clearInvalidationFlagIfSet*/);
    sync_vertices_glyphs(true/*clearInvalidationFlagIfSet*/);
}

void BitmapLabel::disposeMain() {
	//void

    super::disposeMain();
}

BitmapLabel::~BitmapLabel() {
    if(quadsList != nullptr) {
        delete[] quadsList;

        quadsList = nullptr;
    }
}
