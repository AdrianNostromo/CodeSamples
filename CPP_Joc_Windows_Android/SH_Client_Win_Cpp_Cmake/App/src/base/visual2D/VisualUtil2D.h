#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/Container2D.h>
#include <graphics/visual2d/drawable/IContainer2D.h>
#include <graphics/visual2d/drawable/Sprite2D.h>
#include <string>
#include <graphics/visual2d/drawable/TouchArea2D.h>
#include <base/visual2D/util/VisualCreateConfig.h>
#include <graphics/visual2d/drawable/MovieClip2D.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>
#include <graphics/visual2d/drawable/SpriteButton2D.h>
#include <graphics/visual2d/drawable/ITriangles2DDrawable.h>
#include <base/list/Array1D.h>
#include <base/memory/SharedPointer.h>

class IAssetsManager;
class IAppAssets;
class Triangles2DDrawable;
class Slice9SizePercents;
namespace base {
    class Sprite9Slice2D;
};

class VisualUtil2D {
    pub static IDrawableNode2D* newSmartVisual(
        IAppAssets* appAssets,
        VisualCreateConfig* visualCreateConfig,
        IContainer2D* parent,
        IMap1D<std::string, void*>* createConfigMap);

    pub static Container2D* newGroup(
        IContainer2D* parent
    );
    pub static Sprite2D* newSprite(
        IAssetsManager* assetsManager,
        std::string* atlasID, std::string* regionID,
        float posX, float posY,
        float width, float height,
        float anchorX, float anchorY,
        IContainer2D* parent
    );
    pub static Sprite2D* newSprite(
        IAssetsManager* assetsManager,
        std::string* atlasID, std::string* regionID,
        IContainer2D* parent
    );

    pub static base::Sprite9Slice2D* newSprite9Slice2D(
        IAssetsManager* assetsManager,
        bool drawCenterRegion, Slice9SizePercents& sliceSizePercents,
        std::string* atlasID, std::string* regionID,
        IContainer2D* parent
    );

    pub static MovieClip2D* newMovieClip(
        base::IAnimator* animator, IAssetsManager* assetsManager,
        std::string& atlasID, std::string& regionIDPrefix, std::string& regionIDSuffix, int regionIndexStart, int regionIndexEnd, int indexZeroPaddedStringLen, float frameDuration, PlayMode playMode,
        IContainer2D* parent
    );

    pub static TouchArea2D* newTouchArea(
        IAppAssets* app,
        float posX, float posY, float width, float height,
        float anchorX, float anchorY,
		std::string* touchDownSoundConfig_id, float touchDownSound_volume, int touchDown_hapticFeedback_type,
		std::string* touchUpSoundConfig_id, float touchUpSound_volume,
        IContainer2D* parent, ITouchListener* touchListener
    );

    pub static TouchArea2D* newTouchArea(
        IAppAssets* app,
		std::string* touchDownSoundConfig_id, float touchDownSound_volume, int touchDown_hapticFeedback_type,
		std::string* touchUpSoundConfig_id, float touchUpSound_volume,
        IContainer2D* parent);
	
    pub static base::SpriteButton2D* newSpriteButton2D(IAssetsManager* assetsManager,
		std::string& atlasID, std::string& buttonAssetsPrefix,
		std::string* touchDownSoundConfig_id, float touchDownSound_volume,
		std::string* touchUpSoundConfig_id, float touchUpSound_volume,
		std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume,
		IContainer2D* parent);

    pub static base::BitmapLabel* newBitmapLabel(
        IAssetsManager* assetsManager,
        std::string& initialText,
        IContainer2D* parent);
    
    pub static Triangles2DDrawable* newTriangles2D(
        IAssetsManager* assetsManager,
        sp<Array1D<ITriangles2DDrawable::LocalVertex>> verticesList, sp<Array1D<unsigned short>> indicesManaged,
        IContainer2D* parent);

};
