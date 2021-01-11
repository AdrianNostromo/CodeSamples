#include "RasterisingTrueTypeFontLoader.h"
#include <base/assets/util/AssetType.h>
#include <base/assets/loader/TextureLoader.h>
#include <base/assets/loader/GlyphsPackerLoader.h>
#include <base/exceptions/FileNotFoundException.h>
#include <base/exceptions/AssetMissingOrInvalidException.h>
#include <base/util/StringUtil.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/math/Math.h>
#include <base/opengl/util/GLUtil.h>
#include <cstring>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include <vendor/stb/stb_image_write.h>
#include <base/app/config/AppConfig.h>
#include <vendor/stb/stb_rect_pack.h>
#include <base/assets/loader/util/IAssetLoaderHandler.h>

std::string RasterisingTrueTypeFontLoader::AssetID_glyphs = "glyphs";

RasterisingTrueTypeFontLoader::RasterisingTrueTypeFontLoader(
    IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier,
    const std::string& assetsFolderPath,
    std::shared_ptr<LoaderConfig> config, std::string cacheFolderPath)
	:super(handler, identifier, assetsFolderPath),
    config(config),
    cacheFolderPath(cacheFolderPath)
{
    //void
}

int RasterisingTrueTypeFontLoader::tickStatus(int status, bool isStatusInit) {
	switch (status) {
	case LocalStatusCode::Iddle: {
        if (!isStatusInit) {
            //void
        }

        return LocalStatusCode::Loading_PreDeps;
        
        //break;
    }
	case LocalStatusCode::Loading_PreDeps: {
		if (!isStatusInit) {
            initStatus_loading_preDependencies();
		}

        if (isDependenciesLoaded()) {
            return LocalStatusCode::Rasterising;
        }

		break;
    }
	case LocalStatusCode::Rasterising: {
		if (!isStatusInit) {
            initStatus_rasterising();

            return LocalStatusCode::Finished_Success;
		}

        throw LogicException(LOC);

		//break;
    }
	default: {
		return -1;
		//break;
    }}

	return status;
}

void RasterisingTrueTypeFontLoader::initStatus_loading_preDependencies() {
    // Split the composed asset id to parts.
    std::shared_ptr<ArrayList<std::string>> idPartsList = StringUtil::split(getSubPath(), "-", false);

    // Validate the composed asset id parts.
    if(idPartsList->size() != 4) {
        throw LogicException(LOC);
    }
    std::string& fontName = idPartsList->getReference(0);
    std::string& fontWeightName = idPartsList->getReference(1);
    std::string& charactersToRenderId = idPartsList->getReference(2);
    std::string& fontSizeText = idPartsList->getReference(3);

    std::string ttfFileFullPath = getAssetsFolderPath() + fontName + "-" + fontWeightName + ".ttf";

    // Eg: "fonts/Roboto-Medium-AaNSS-32";
    fontName = fontName + "-" + fontWeightName + "-" + charactersToRenderId + "-" + fontSizeText;

    charactersToRender = &VCC_BitmapLabel::CharacterRanges::fromId(charactersToRenderId);
    fontSize = StringUtil::parseInt(fontSizeText);
    if(fontSize <= 0 || fontSize > MAX_FONT_SIZE) {
        throw LogicException(LOC);
    }

    // Load the font ".ttf" file.
    std::shared_ptr<base::IFileHandlerBlocking> fh = base::Util_FileSystem::fs->getFileHandlerBlocking(ttfFileFullPath, getFsTarget());
    try {
        buff = fh->readBinaryP();
    }catch(FileNotFoundException& x) {
        throw AssetMissingOrInvalidException();
    }
    if(buff == nullptr) {
        throw LogicException(LOC);
    }
    
    // Add a preDep for the glyphs packer with id "glyphs".
    std::shared_ptr<GlyphsPackerLoader::LoaderConfig> loaderConfig = std::make_shared<GlyphsPackerLoader::LoaderConfig>(config->globalTextures);

    std::shared_ptr<AssetLoaderDependency> dep = std::make_shared<AssetLoaderDependency>(
        AssetIdentifier::getOrCreate(AssetType::GlyphsPacker, AssetID_glyphs), base::Util_FileSystem::FSTarget::Private,
        loaderConfig,
        getAssetsFolderPath()
    );
    pushDependency(dep);
}

void RasterisingTrueTypeFontLoader::initStatus_rasterising() {
    char* ttf_buffer = buff->getBlockCharPointer();

    // Parse data to a font structure.
    stbtt_fontinfo font;

    int r = stbtt_InitFont(
        &font,
        (unsigned char*)ttf_buffer,
        stbtt_GetFontOffsetForIndex((unsigned char*)ttf_buffer, 0)
    );
    if (r == 0) {
        throw LogicException(LOC);
    }

    float fontScaleFactor = stbtt_ScaleForPixelHeight(&font, fontSize);

    {
        int x0;
        int y0;
        int x1;
        int y1;
        stbtt_GetFontBoundingBox(
            &font,
            &x0, &y0, &x1, &y1
        );

        // Note. These are in unscaled coordinates, so you must multiply by the scale factor for a given size.
        int unscaledAscent;
        int unscaledDescent;
        int unscaledLineGap;
        stbtt_GetFontVMetrics(
            &font,
            &unscaledAscent, &unscaledDescent, &unscaledLineGap
        );

        int scaledAscent = Math::round(unscaledAscent * fontScaleFactor);
        int scaledDescent = Math::round(unscaledDescent * fontScaleFactor);
        int scaledLineGap = Math::round(unscaledLineGap * fontScaleFactor);

        // Old improvised method (looks good visually but is not exact). 
        // int lineHeight = (int)Math::ceil((y1 - y0) * fontScaleFactor);
        int lineHeight = Math::ceil(scaledAscent - scaledDescent + scaledLineGap);

        // Convert baseline to bottom left origin.
        // Old improvised method (looks good visually but is not exact). 
        // int baseline = (int)Math::ceil(-y0 * fontScaleFactor);
        int baseline = -scaledDescent;

        // Currently kernings are not implemented.
        int kerningsCount = 0;

        data = newt base::BitmapFont(
            fontName,
            fontSize,
            lineHeight, baseline,
            scaledAscent, scaledDescent, scaledLineGap,
            kerningsCount
        );
    }

    std::shared_ptr<AssetLoaderDependency> depAsset = getDependency(AssetType::GlyphsPacker, AssetID_glyphs);
    if (depAsset == nullptr) {
        // The glyphs must always exist because it is created if files don't exist.
        throw LogicException(LOC);
    }
    
    base::GlyphsPacker* glyphsPacker = depAsset->asset->getAsGlyphsPacker();
    
    packFontBitmap(
        fontSize, *charactersToRender, ttf_buffer,
        glyphsPacker,
        data
    );

    cleanup();
}

void RasterisingTrueTypeFontLoader::writeFontGlyph(
    base::BitmapFont* fontAsset, base::IGlyphsPacker* glyphsPacker,
    int unicodeCodepointDecimal, stbtt_packedchar* packedCharData,
    unsigned char* glyphixels, int glyphX, int glyphY, int glyphWidth, int glyphHeight, int padding)
{
    int width = (int)(packedCharData->x1 - packedCharData->x0);
    int height = (int)(packedCharData->y1 - packedCharData->y0);
    int offsetX = (int)packedCharData->xoff;
    int offsetY = fontAsset->baseline - (int)packedCharData->yoff2;
    int advanceX = (int)Math::ceil(packedCharData->xadvance);
    // Currently fonts can have only 1 texture page.
    int pageIndex = -1;

    TextureRegion* textureRegion = nullptr;
    if (width != 0 && height != 0) {
        sp<base::IGlyphsPacker::ComposedGlyphRegion> textureRegionInfo = glyphsPacker->getFontTextureRegion(width, height, padding);
        if (textureRegionInfo == nullptr) {
            throw LogicException(LOC);
        }

        textureRegion = textureRegionInfo->textureRegion;
        if (textureRegion->getPixelsWidth() <= 0 || textureRegion->getPixelsHeight() <= 0) {
            // Inverted regions are not supported by the copy code. Use normal regions.
            throw LogicException(LOC);
        }
        
        if (textureRegionInfo->textureAsset != lastGlyphPackingTextureAsset && !getLoadingAsset()->asset->getSubDependenciesList()->containsDirect(textureRegionInfo->textureAsset->getIdentifier())) {
            getLoadingAsset()->asset->connectToSubDependencyAsset(textureRegionInfo->textureAsset);
        }
        
        lastGlyphPackingTextureAsset = textureRegionInfo->textureAsset;
        base::Buffer pixels4Channels = textureRegionInfo->textureRegion->getTexture()->getDataUnpacked(false/*unpackIfNeeded*/, true/*mustExist*/);
        
        copyGlyph1CHannelTo4ChannelsFlipY(
            glyphixels, glyphWidth, glyphX/*inRegionX*/, glyphY/*inRegionY*/,
            static_cast<unsigned char*>(pixels4Channels.getBlock()), 
            textureRegion->getTexture()->getWidth(), textureRegion->getTexture()->getHeight(), 
            textureRegion->getPixelsX()/*outRegionX*/, textureRegion->getPixelsY()/*outRegionY*/,
            glyphWidth, glyphHeight
        );

        pageIndex = fontAsset->getOrCreatePageIndex(textureRegion->getTexture());
    }

    base::BitmapFont::Glyph* glyph = newt base::BitmapFont::Glyph(
        unicodeCodepointDecimal,
        pageIndex,
        textureRegion,
        offsetX, offsetY,
        width, height,
        advanceX
    );

    fontAsset->initGlyph(unicodeCodepointDecimal, glyph);
}

void RasterisingTrueTypeFontLoader::packFontBitmap(
    int fontSize, std::string& charactersToRender, char* ttf_buffer, 
    base::IGlyphsPacker* glyphsPacker, base::BitmapFont* fontAsset)
{
    int padding = 1;
    unsigned int h_oversample = 1;
    unsigned int v_oversample = 1;

    const unsigned char* fontdata = (unsigned char*)ttf_buffer;
    int font_index = 0;

    stbtt_fontinfo info;
    stbtt_InitFont(&info, fontdata, stbtt_GetFontOffsetForIndex(fontdata, font_index));

    float fh = fontSize;
    float scale = fh > 0 ? stbtt_ScaleForPixelHeight(&info, fh) : stbtt_ScaleForMappingEmToPixels(&info, -fh);

    float recip_h, recip_v, sub_x, sub_y;
    recip_h = 1.0f / h_oversample;
    recip_v = 1.0f / v_oversample;
    sub_x = stbtt__oversample_shift(h_oversample);
    sub_y = stbtt__oversample_shift(v_oversample);

    int glyphWidth;
    int glyphHeight;

    // This is resized as needed.
    unsigned char* glyphixels = nullptr;
    int glyphixelsCount = 0;

    int gX = 0;
    int gY = 0;
    for (int i = 0; i < charactersToRender.size(); i++) {
        int codepoint = static_cast<unsigned char>(charactersToRender[i]);
        if (codepoint < 0) {
            // A cast error occured.
            throw LogicException(LOC);
        }

        int glyph = stbtt_FindGlyphIndex(&info, codepoint);
        int x0, y0, x1, y1;

        stbtt_GetGlyphBitmapBoxSubpixel(&info, glyph,
            scale * h_oversample,
            scale * v_oversample,
            0, 0,
            &x0, &y0, &x1, &y1);

        if (glyph != 0) {
            glyphWidth = (stbrp_coord)(x1 - x0 + h_oversample - 1);
            glyphHeight = (stbrp_coord)(y1 - y0 + v_oversample - 1);
        } else {
            glyphWidth = glyphHeight = 0;
        }

        int advance, lsb;

        // pad on left and top
        stbtt_GetGlyphHMetrics(&info, glyph, &advance, &lsb);
        //This seems redundant currently.
        stbtt_GetGlyphBitmapBox(&info, glyph,
            scale * h_oversample,
            scale * v_oversample,
            &x0, &y0, &x1, &y1);

        int stride_in_bytes = glyphWidth;

        if (glyphWidth * glyphHeight > glyphixelsCount) {
            if (glyphixels != nullptr) {
                free(glyphixels);
                glyphixels = nullptr;
            }

            // Add a multiplier of 1.5 to minimise the realocations count.
            glyphixelsCount = glyphWidth * glyphHeight * 1.5;
            glyphixels = (unsigned char*)malloc(glyphixelsCount);
        }

        memset(glyphixels, 0, glyphWidth * glyphHeight);

        stbtt_MakeGlyphBitmapSubpixel(&info,
            glyphixels/* + r.x + r.y * stride_in_bytes*//*start_rendering_at_start_of_pixels*/,
            glyphWidth - h_oversample + 1,
            glyphHeight - v_oversample + 1,
            stride_in_bytes,
            scale * h_oversample,
            scale * v_oversample,
            0, 0,
            glyph
        );

        if (h_oversample > 1) {
            stbtt__h_prefilter(glyphixels/* + r.x + r.y * stride_in_bytes*//*start_rendering_at_start_of_pixels*/,
                glyphWidth, glyphHeight, stride_in_bytes,
                h_oversample
            );
        }

        if (v_oversample > 1) {
            stbtt__v_prefilter(glyphixels/* + r.x + r.y * stride_in_bytes*//*start_rendering_at_start_of_pixels*/,
                glyphWidth, glyphHeight, stride_in_bytes,
                v_oversample
            );
        }

        stbtt_packedchar packedCharData{};
        memset(&packedCharData, 0, sizeof(stbtt_packedchar));

        packedCharData.x0 = (unsigned short)0;
        packedCharData.y0 = (unsigned short)0;
        packedCharData.x1 = (unsigned short)(0 + glyphWidth);
        packedCharData.y1 = (unsigned short)(0 + glyphHeight);
        packedCharData.xadvance = scale * advance;
        packedCharData.xoff = (float)x0 * recip_h + sub_x;
        packedCharData.yoff = (float)y0 * recip_v + sub_y;
        packedCharData.xoff2 = (x0 + glyphWidth) * recip_h + sub_x;
        packedCharData.yoff2 = (y0 + glyphHeight) * recip_v + sub_y;

        writeFontGlyph(
            fontAsset, glyphsPacker,
            codepoint/*unicodeCodepointDecimal*/, &packedCharData,
            glyphixels, 0/*glyphX*/, 0/*glyphY*/, glyphWidth, glyphHeight, padding
        );
    }

    for (int i = 0; i < fontAsset->peekPageTexturesList()->count(); i++) {
        Texture* texture = fontAsset->peekPageTexturesList()->getDirect(i);

        texture->invalidateData();
    }

    if (glyphixels != nullptr) {
        free(glyphixels);
        glyphixels = nullptr;
    }
}

void RasterisingTrueTypeFontLoader::copyGlyph1CHannelTo4ChannelsFlipY(
    unsigned char* inPixels1Channel, int inImageW, int inRegionX, int inRegionY,
    unsigned char* outPixels4Channels, int outImageW, int outImageH, int outRegionX, int outRegionY,
    int regionW, int regionH)
{
    for (int lineIndex = 0; lineIndex < regionH; lineIndex++) {
        int inLineGY = inRegionY + lineIndex;
        int outLineGY = outRegionY + regionH - 1 - lineIndex;

        // This is fliped_y;
        unsigned char* inScanlinePixels = &inPixels1Channel[inLineGY * inImageW + inRegionX];
        // This flips in the entire image, not just in the glyph box.
        int* outScanlinePixels = reinterpret_cast<int*>(&outPixels4Channels[(outLineGY * outImageW + outRegionX) * 4]);

        for (int i = 0; i < regionW; i++) {
            unsigned char px = inScanlinePixels[i];

            // The bytes will be written in reverse order (duh).
            int col = 0x00FFFFFF | (px << 24);
            
            outScanlinePixels[i] = col;
        }
    }
}

std::string RasterisingTrueTypeFontLoader::computeAtlasFolderPath() {
	const std::string& atlasPath = getSubPath();

	std::size_t found = atlasPath.find_last_of("/\\");

	std::string atlasFolderPath = atlasPath.substr(0, found + 1);

	return atlasFolderPath;
}

void* RasterisingTrueTypeFontLoader::extractData() {
    base::BitmapFont* t = data;

    data = nullptr;

    return t;
}

void RasterisingTrueTypeFontLoader::cleanup() {
    if (buff != nullptr) {
        delete buff;

        buff = nullptr;
    }
    if(packedCharsData != nullptr) {
        delete[] packedCharsData;

        packedCharsData = nullptr;
    }
}

void RasterisingTrueTypeFontLoader::disposeMain() {
    cleanup();

    super::disposeMain();
}

RasterisingTrueTypeFontLoader::~RasterisingTrueTypeFontLoader() {
    if (data) {
        delete data;

        data = nullptr;
    }
}
