#include "VisualUtil2D.h"
#include <graphics/visual2d/drawable/Container2D.h>
#include <graphics/visual/TextureRegion.h>
#include <base/exceptions/LogicException.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_Sprite9Slice2D.h>
#include <base/geometry/Circle.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_SpriteButton.h>
#include <base/visual2D/util/VCC_MovieClip.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/visual2D/util/VCC_Triangles2D.h>
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>
#include <base/sensor/hapticFeedback/util/HapticFeedbackType.h>
#include <base/sensor/hapticFeedback/HapticFeedback.h>
#include <base/nativeInput/INativeInput.h>
#include <graphics/model/RenderableLiteInstanceVIMN.h>
#include <graphics/util/GraphicsUtil.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include <graphics/visual2d/drawable/Triangles2DDrawable.h>
#include <graphics/visual2d/drawable/Sprite9Slice2D.h>

IDrawableNode2D* VisualUtil2D::newSmartVisual(
    IAppAssets* appAssets,
    VisualCreateConfig* visualCreateConfig,
    IContainer2D* parent,
    IMap1D<std::string, void*>* createConfigMap)
{
    if(visualCreateConfig == nullptr) {
        throw LogicException(LOC);
    }

    IDrawableNode2D* visual = nullptr;
    if(visualCreateConfig->type == VCC_Group::TYPE) {
        VCC_Group* localizedConfig = dynamic_cast<VCC_Group*>(visualCreateConfig);
        (void)localizedConfig;
        visual = VisualUtil2D::newGroup(parent);
    }else if(visualCreateConfig->type == VCC_Sprite::TYPE) {
        VCC_Sprite* localizedConfig = dynamic_cast<VCC_Sprite*>(visualCreateConfig);

        std::string* atlasID = nullptr;
        std::string* regionID = nullptr;
        if (localizedConfig->managedAtlasPath != nullptr) {
            atlasID = &localizedConfig->managedAtlasPath->path;
            regionID = &localizedConfig->regionID;
        }

        visual = VisualUtil2D::newSprite(
            appAssets->getEmbeddedAssetsManager(),
            atlasID, regionID,
            parent
        );
    }else if(visualCreateConfig->type == VCC_Sprite9Slice2D::TYPE) {
        VCC_Sprite9Slice2D* localizedConfig = dynamic_cast<VCC_Sprite9Slice2D*>(visualCreateConfig);

        std::string* atlasID = nullptr;
        std::string* regionID = nullptr;
		if (localizedConfig->managedAtlasPath != nullptr) {
            atlasID = &localizedConfig->managedAtlasPath->path;
            regionID = &localizedConfig->regionID;
		}

        visual = VisualUtil2D::newSprite9Slice2D(
            appAssets->getEmbeddedAssetsManager(),
            localizedConfig->drawCenterRegion, localizedConfig->sliceSizePercents,
            atlasID, regionID,
            parent
        );
    }else if(visualCreateConfig->type == VCC_TouchArea::TYPE) {
        VCC_TouchArea* localizedConfig = dynamic_cast<VCC_TouchArea*>(visualCreateConfig);

        visual = VisualUtil2D::newTouchArea(
            appAssets,
            localizedConfig->touchDownSoundConfig_id, localizedConfig->touchDownSound_volume, localizedConfig->touchDown_hapticFeedback_type,
            localizedConfig->touchUpSoundConfig_id, localizedConfig->touchUpSound_volume,
            parent
        );
    }else if(visualCreateConfig->type == VCC_SpriteButton::TYPE) {
        VCC_SpriteButton* localizedConfig = dynamic_cast<VCC_SpriteButton*>(visualCreateConfig);

        visual = VisualUtil2D::newSpriteButton2D(
			appAssets->getEmbeddedAssetsManager(), 
            localizedConfig->managedAtlasPath != nullptr ? localizedConfig->managedAtlasPath->path : StringUtil::EMPTY, localizedConfig->buttonAssetsPrefix,
            localizedConfig->touchDownSoundConfig_id, localizedConfig->touchDownSound_volume,
            localizedConfig->touchUpSoundConfig_id, localizedConfig->touchUpSound_volume,
			localizedConfig->disabledTouchDownSoundConfig_id, localizedConfig->disabledTouchDownSound_volume,
            parent
        );
    }else if(visualCreateConfig->type == VCC_MovieClip::TYPE) {
        VCC_MovieClip* localizedConfig = dynamic_cast<VCC_MovieClip*>(visualCreateConfig);

        MovieClip2D* lVisual = VisualUtil2D::newMovieClip(
            appAssets->getAnimator(), appAssets->getEmbeddedAssetsManager(),
            localizedConfig->managedAtlasPath->path, localizedConfig->regionIDPrefix, localizedConfig->regionIDSuffix, localizedConfig->regionIndexStart, localizedConfig->regionIndexEnd, localizedConfig->indexZeroPaddedStringLen, localizedConfig->frameDuration, localizedConfig->playMode,
            parent
        );
        visual = lVisual;

        if(!localizedConfig->isPlaying) {
            lVisual->stop();
        }else {
            lVisual->play(false);
        }
    }else if(visualCreateConfig->type == VCC_BitmapLabel::TYPE) {
        VCC_BitmapLabel* localizedConfig = dynamic_cast<VCC_BitmapLabel*>(visualCreateConfig);

        base::BitmapLabel* lVisual = VisualUtil2D::newBitmapLabel(
            appAssets->getEmbeddedAssetsManager(),
            localizedConfig->initialText,
            parent
        );
        visual = lVisual;
    }else if(visualCreateConfig->type == VCC_Triangles2D::TYPE) {
        VCC_Triangles2D* localizedConfig = dynamic_cast<VCC_Triangles2D*>(visualCreateConfig);

        Triangles2DDrawable* lVisual = VisualUtil2D::newTriangles2D(
            appAssets->getEmbeddedAssetsManager(),
            localizedConfig->verticesList, localizedConfig->indicesManaged,
            parent
        );
        
        visual = lVisual;
    }/*asdB// implement as needed;
    else if(visualCreateConfig.type == VisualCreateConfig_MovieClipButton::TYPE) {
        VisualCreateConfig_MovieClipButton localizedConfig = (VisualCreateConfig_MovieClipButton)visualCreateConfig;

        visual = VisualUtil.newMovieClipButton(appAssets.getEmbeddedAssetsManager(), appAssets.getSoundManager(), localizedConfig.managedAtlasPath.path, localizedConfig.regionIDPrefix, localizedConfig.regionIndexStart, localizedConfig.regionIndexEnd, localizedConfig.indexZeroPaddedStringLen, localizedConfig.frameDuration, localizedConfig.playMode, localizedConfig.touchDownSoundConfig, localizedConfig.touchUpSoundConfig, parent);
        if(!localizedConfig.isPlaying) {
            ((MovieClipButton) visual).stop();
        }else {
            ((MovieClipButton)visual).play(false);
        }
    }else if(visualCreateConfig.type == VisualCreateConfig_Button::TYPE) {
        VisualCreateConfig_Button localizedConfig = (VisualCreateConfig_Button)visualCreateConfig;

        visual = VisualUtil.newButton(appAssets.getEmbeddedAssetsManager(), appAssets.getSoundManager(), localizedConfig.managedAtlasPath.path, localizedConfig.buttonAssetsPrefix, parent, null, localizedConfig.touchDownSoundConfig, localizedConfig.touchUpSoundConfig, localizedConfig.disabledTouchDownSoundConfig);
    }
    else if(visualCreateConfig.type == VCC_BitmapLabelIndexedFonts::TYPE) {
        VCC_BitmapLabelIndexedFonts localizedConfig = (VCC_BitmapLabelIndexedFonts)visualCreateConfig;

        FontConfig[] fcList = newt FontConfig[localizedConfig.fontDataList.length];
        for(int i=0;i<localizedConfig.fontDataList.length;i++) {
            Object fontData = localizedConfig.fontDataList[i];

            FontConfig fc;
            if(fontData instanceof FontConfig) {
                fc = (FontConfig)fontData;
            }else if(fontData instanceof String){
                fc = FontConfig.entriesMap.get((String)fontData);
            }else {
                throw LogicException(LOC);
            }
            fcList[i] = fc;
        }
        visual = VisualUtil.newLabelIndexedFonts(
            appAssets.getFontsManager(),
            fcList, localizedConfig.defaultText,
            parent
        );
    }else if(visualCreateConfig.type == VisualCreateConfig_CustomRectangleSprite::TYPE) {
        VisualCreateConfig_CustomRectangleSprite localizedConfig = (VisualCreateConfig_CustomRectangleSprite)visualCreateConfig;

        Integer color;
        if(localizedConfig.color instanceof String) {
            color = (Integer) createConfigMap.get((String) localizedConfig.color);
        }else if(localizedConfig.color instanceof Integer) {
            color = (Integer)localizedConfig.color;
        }else {
            throw LogicException(LOC);
        }

        visual = VisualUtil.newCustomRectangleSprite(appAssets.getEmbeddedAssetsManager(), color, localizedConfig.textureSize, parent);
    }else if(visualCreateConfig.type == VisualCreateConfig_PixelSprite::TYPE) {
        VisualCreateConfig_PixelSprite localizedConfig = (VisualCreateConfig_PixelSprite)visualCreateConfig;

        visual = VisualUtil.newPixelSprite(localizedConfig.pixelData, parent);
    }else if(visualCreateConfig.type == VisualCreateConfig_ConeTrailSprite2D::TYPE) {
        VisualCreateConfig_ConeTrailSprite2D localizedConfig = (VisualCreateConfig_ConeTrailSprite2D)visualCreateConfig;

        visual = VisualUtil.newConeTrailSprite2D(appAssets.getEmbeddedAssetsManager(), localizedConfig.managedAtlasPath.path, localizedConfig.regionID, parent);
    }*/else {
        throw LogicException(LOC);
    }

    if(dynamic_cast<VCC_Touchable*>(visualCreateConfig)) {
        // Set touch shape.
        if((dynamic_cast<VCC_Touchable*>(visualCreateConfig))->touchShape == VCC_Touchable::TouchShape::Rectangle) {
            (dynamic_cast<IInteractiveDrawable2D*>(visual))->setTouchShape(std::static_pointer_cast<Shape2D>(std::make_shared<Rectangle>()));
        }else if((dynamic_cast<VCC_Touchable*>(visualCreateConfig))->touchShape == VCC_Touchable::TouchShape::Circle) {
            (dynamic_cast<IInteractiveDrawable2D*>(visual))->setTouchShape(std::static_pointer_cast<Shape2D>(std::make_shared<Circle>()));
        }else {
            throw LogicException(LOC);
        }
    }

    return visual;
}

Container2D* VisualUtil2D::newGroup(IContainer2D* parent) {
	Container2D* spritesVisualsGroup = newt Container2D();
    spritesVisualsGroup->reservedCreate();

	if (parent) {
		parent->addChild(spritesVisualsGroup);
	}

	return spritesVisualsGroup;
}

Sprite2D* VisualUtil2D::newSprite(
	IAssetsManager* assetsManager,
	std::string* atlasID, std::string* regionID,
	float posX, float posY,
	float width, float height,
	float anchorX, float anchorY,
	IContainer2D* parent
) {
	Sprite2D* sprite = newSprite(assetsManager, atlasID, regionID, parent);
	if (sprite == nullptr) {
		return nullptr;
	}

	sprite->setSize(width, height);
	sprite->setPosition(posX, posY);
	sprite->setAnchor(anchorX, anchorY);

	return sprite;
}

Sprite2D* VisualUtil2D::newSprite(
	IAssetsManager* assetsManager,
	std::string* atlasID, std::string* regionID,
	IContainer2D* parent
) {
	TextureRegion* region = nullptr;
	if (atlasID != nullptr) {
		// The texture is configured to exist.
		region = assetsManager->getTextureAtlasRegion(*atlasID, *regionID);
		if (region == nullptr) {
			throw LogicException(LOC);
		}
	}

    Sprite2D* sprite = newt Sprite2D(region);
    sprite->reservedCreate();

	if (parent != nullptr) {
		parent->addChild(sprite);
	}

	return sprite;
}

base::Sprite9Slice2D* VisualUtil2D::newSprite9Slice2D(
    IAssetsManager* assetsManager,
    bool drawCenterRegion, Slice9SizePercents& sliceSizePercents,
    std::string* atlasID, std::string* regionID,
    IContainer2D* parent)
{
    TextureRegion* region = nullptr;
    if (atlasID != nullptr) {
        // The texture is configured to exist.
        region = assetsManager->getTextureAtlasRegion(*atlasID, *regionID);
        if (region == nullptr) {
            throw LogicException(LOC);
        }
    }

    base::Sprite9Slice2D* sprite = newt base::Sprite9Slice2D(
        drawCenterRegion,
        region, &sliceSizePercents
    );
    sprite->reservedCreate();

    if (parent != nullptr) {
        parent->addChild(sprite);
    }

    return sprite;
}

MovieClip2D* VisualUtil2D::newMovieClip(
    base::IAnimator* animator, IAssetsManager* assetsManager,
    std::string& atlasID, std::string& regionIDPrefix, std::string& regionIDSuffix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen, float frameDuration, PlayMode playMode,
    IContainer2D* parent)
{
    std::shared_ptr<ArrayList<TextureRegion*>> keyFrames = assetsManager->getTextureAtlasRegionsArray(
        atlasID,
        regionIDPrefix, regionIDSuffix, regionIndexStart, regionIndexEnd, indexZeroPaddedStringLen);

    if(keyFrames->size() < 2) {
        // Error. MovieClips must have at least 2 frames.
        throw LogicException(LOC);
    }

    std::shared_ptr<base::AnimatedList<TextureRegion*>> framesAnimatedList = std::make_shared<base::AnimatedList<TextureRegion*>>(frameDuration, keyFrames, playMode);
    MovieClip2D* mc = newt MovieClip2D(animator, framesAnimatedList);
    mc->reservedCreate();

    if(parent != nullptr) {
        parent->addChild(mc);
    }

    return mc;
}

TouchArea2D* VisualUtil2D::newTouchArea(
    IAppAssets* app,
    float posX, float posY, float width, float height,
    float anchorX, float anchorY,
    std::string* touchDownSoundConfig_id, float touchDownSound_volume, int touchDown_hapticFeedback_type,
    std::string* touchUpSoundConfig_id, float touchUpSound_volume,
    IContainer2D* parent, ITouchListener* touchListener)
{
    TouchArea2D* area2D = newTouchArea(
        app,
        touchDownSoundConfig_id, touchDownSound_volume, touchDown_hapticFeedback_type,
        touchUpSoundConfig_id, touchUpSound_volume,
        parent
    );

    area2D->setSize(width, height);
    area2D->setAnchor(anchorX, anchorY);
    area2D->setPosition(posX, posY);

    if(touchListener != nullptr) {
        area2D->addTouchListener(touchListener);
    }

    return area2D;
}

TouchArea2D* VisualUtil2D::newTouchArea(
    IAppAssets* app,
    std::string* touchDownSoundConfig_id, float touchDownSound_volume, int touchDown_hapticFeedback_type,
    std::string* touchUpSoundConfig_id, float touchUpSound_volume,
    IContainer2D* parent)
{
	base::audio::ISound* touchDownSound = nullptr;
	base::audio::ISound* touchUpSound = nullptr;
    if(touchDownSoundConfig_id != nullptr) {
        touchDownSound = app->getEmbeddedAssetsManager()->getSound(*touchDownSoundConfig_id);
    }
    if(touchUpSoundConfig_id != nullptr) {
        touchUpSound = app->getEmbeddedAssetsManager()->getSound(*touchUpSoundConfig_id);
    }

    base::HapticFeedback* touchDown_hapticFeedback = nullptr;
    if(touchDown_hapticFeedback_type != base::HapticFeedbackType::NONE && app->getNativeInput()->getIsHapticFeedbackSupported()) {
        touchDown_hapticFeedback = newt base::HapticFeedback(app->getNativeInput(), touchDown_hapticFeedback_type);
    }

    TouchArea2D* area2D = newt TouchArea2D(
        touchDownSound, touchDownSound_volume, touchDown_hapticFeedback,
        touchUpSound, touchUpSound_volume
    );
    area2D->reservedCreate();

    area2D->setIsInputInteractive(true);

    if(parent != nullptr) {
        parent->addChild(area2D);
    }

    return area2D;
}

base::SpriteButton2D* VisualUtil2D::newSpriteButton2D(
	IAssetsManager* assetsManager, 
	std::string& atlasID, std::string& buttonAssetsPrefix,
    std::string* touchDownSoundConfig_id, float touchDownSound_volume,
    std::string* touchUpSoundConfig_id, float touchUpSound_volume,
    std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume,
    IContainer2D* parent)
{
	base::audio::ISound* touchDownSound = nullptr;
	base::audio::ISound* touchUpSound = nullptr;
	base::audio::ISound* disabledTouchDownSound = nullptr;
    if(touchDownSoundConfig_id != nullptr) {
        touchDownSound = assetsManager->getSound(*touchDownSoundConfig_id);
    }
    if(touchUpSoundConfig_id != nullptr) {
        touchUpSound = assetsManager->getSound(*touchUpSoundConfig_id);
    }
    if(disabledTouchDownSoundConfig_id != nullptr) {
		disabledTouchDownSound = assetsManager->getSound(*disabledTouchDownSoundConfig_id);
    }

	base::SpriteButton2D* item = newt base::SpriteButton2D(
		touchDownSound, touchDownSound_volume,
		touchUpSound, touchUpSound_volume,
		disabledTouchDownSound, disabledTouchDownSound_volume,
        atlasID, buttonAssetsPrefix,
        assetsManager
	);
	item->reservedCreate();

	item->setIsInputInteractive(true);

    if(parent != nullptr) {
        parent->addChild(item);
    }

    return item;
}

base::BitmapLabel* VisualUtil2D::newBitmapLabel(
    IAssetsManager* assetsManager,
    std::string& initialText,
    IContainer2D* parent)
{
    //asd_011;// use the new method and remove the fontId.

	//asdA34;// Load if missing. This occurs becase of windows size change causes dpm changes.
	//asdA34;// Make the font_size a property that can be animated and stuff.
	//asdA34;// Include config in the view label for some fonts to be preloaded and the rest are loaded on the fly in blocking mode or delayed_rendering mode.

    base::BitmapLabel* bitmapLabel = newt base::BitmapLabel(assetsManager/*fontProvider*/);
    bitmapLabel->reservedCreate();

    bitmapLabel->setTextDirect(initialText);

    if(parent != nullptr) {
        parent->addChild(bitmapLabel);
    }

    return bitmapLabel;
}

Triangles2DDrawable* VisualUtil2D::newTriangles2D(
    IAssetsManager* assetsManager,
    sp<Array1D<ITriangles2DDrawable::LocalVertex>> verticesList, sp<Array1D<unsigned short>> indicesManaged,
    IContainer2D* parent)
{
    Triangles2DDrawable* drawable = newt Triangles2DDrawable();
    drawable->reservedCreate();

    if (verticesList != nullptr || indicesManaged != nullptr) {
        if (verticesList == nullptr || indicesManaged == nullptr) {
            // Both vars mut be set or nullptr;
            throw LogicException(LOC);
        }

        drawable->setLocalGeometry(verticesList, indicesManaged);
    }

    if(parent != nullptr) {
        parent->addChild(drawable);
    }

    return drawable;
}
