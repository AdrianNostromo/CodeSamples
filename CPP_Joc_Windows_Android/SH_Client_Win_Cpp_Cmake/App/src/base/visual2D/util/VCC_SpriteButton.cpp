#include "VCC_SpriteButton.h"

int VCC_SpriteButton::TYPE = super::NewType();

VCC_SpriteButton::VCC_SpriteButton(
	ManagedAssetPath* managedAtlasPath, std::string buttonAssetsPrefix,
	std::string* touchDownSoundConfig_id, float touchDownSound_volume,
	std::string* touchUpSoundConfig_id, float touchUpSound_volume,
	std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume, int touchShape)
    : super(TYPE, touchShape),
      managedAtlasPath(managedAtlasPath), buttonAssetsPrefix(buttonAssetsPrefix),
      touchDownSoundConfig_id(touchDownSoundConfig_id), touchDownSound_volume(touchDownSound_volume),
      touchUpSoundConfig_id(touchUpSoundConfig_id), touchUpSound_volume(touchUpSound_volume),
      disabledTouchDownSoundConfig_id(disabledTouchDownSoundConfig_id), disabledTouchDownSound_volume(disabledTouchDownSound_volume)
{
    //void
}

VCC_SpriteButton::VCC_SpriteButton(
	ManagedAssetPath* managedAtlasPath, std::string buttonAssetsPrefix,
	std::string* touchDownSoundConfig_id, float touchDownSound_volume,
	std::string* touchUpSoundConfig_id, float touchUpSound_volume,
	std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume)
    : VCC_SpriteButton(
    	managedAtlasPath, buttonAssetsPrefix,
    	touchDownSoundConfig_id, touchDownSound_volume,
    	touchUpSoundConfig_id, touchUpSound_volume,
    	disabledTouchDownSoundConfig_id, disabledTouchDownSound_volume,
    	TouchShape::Rectangle)
{
	//void
}

VCC_SpriteButton::VCC_SpriteButton(
	std::string atlasPath, std::string buttonAssetsPrefix,
	std::string* touchDownSoundConfig_id, float touchDownSound_volume,
	std::string* touchUpSoundConfig_id, float touchUpSound_volume,
	std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume,
	int touchShape)
    : VCC_SpriteButton(
		atlasPath.length() > 0 ? ManagedAssetPath::stringPathToManagedTextureAtlasPath(atlasPath) : nullptr, buttonAssetsPrefix,
		touchDownSoundConfig_id, touchDownSound_volume,
		touchUpSoundConfig_id, touchUpSound_volume,
		disabledTouchDownSoundConfig_id, disabledTouchDownSound_volume,
    	touchShape)
{
	//void
}

VCC_SpriteButton* VCC_SpriteButton::New(
	ManagedAssetPath* managedAtlasPath, std::string buttonAssetsPrefix,
	std::string* touchDownSoundConfig_id, float touchDownSound_volume,
	std::string* touchUpSoundConfig_id, float touchUpSound_volume,
	std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume,
	int touchShape)
{
	VCC_SpriteButton* ret = new VCC_SpriteButton(
		managedAtlasPath,
		buttonAssetsPrefix,
		touchDownSoundConfig_id, touchDownSound_volume,
		touchUpSoundConfig_id, touchUpSound_volume,
		disabledTouchDownSoundConfig_id, disabledTouchDownSound_volume,
		touchShape);

	return ret;
}

VCC_SpriteButton* VCC_SpriteButton::New(
	ManagedAssetPath* managedAtlasPath, std::string buttonAssetsPrefix,
	std::string* touchDownSoundConfig_id, float touchDownSound_volume,
	std::string* touchUpSoundConfig_id, float touchUpSound_volume,
	std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume)
{
	VCC_SpriteButton* ret = new VCC_SpriteButton(
		managedAtlasPath, buttonAssetsPrefix,
		touchDownSoundConfig_id, touchDownSound_volume,
		touchUpSoundConfig_id, touchUpSound_volume,
		disabledTouchDownSoundConfig_id, disabledTouchDownSound_volume);

	return ret;
}

VCC_SpriteButton* VCC_SpriteButton::New(
	std::string atlasPath, std::string buttonAssetsPrefix,
	std::string* touchDownSoundConfig_id, float touchDownSound_volume,
	std::string* touchUpSoundConfig_id, float touchUpSound_volume,
	std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume,
	int touchShapeS)
{
	VCC_SpriteButton* ret = new VCC_SpriteButton(
		atlasPath, buttonAssetsPrefix,
		touchDownSoundConfig_id, touchDownSound_volume,
		touchUpSoundConfig_id, touchUpSound_volume,
		disabledTouchDownSoundConfig_id, disabledTouchDownSound_volume,
		touchShapeS);

	return ret;
}

VCC_SpriteButton* VCC_SpriteButton::New(
	std::string atlasPath, std::string buttonAssetsPrefix,
	std::string* touchDownSoundConfig_id, float touchDownSound_volume,
	std::string* touchUpSoundConfig_id, float touchUpSound_volume,
	std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume)
{
	VCC_SpriteButton* ret = new VCC_SpriteButton(
		atlasPath, buttonAssetsPrefix,
		touchDownSoundConfig_id, touchDownSound_volume,
		touchUpSoundConfig_id, touchUpSound_volume,
		disabledTouchDownSoundConfig_id, disabledTouchDownSound_volume,
		TouchShape::Rectangle);

	return ret;
}

VCC_SpriteButton::~VCC_SpriteButton() {
    // void
}
