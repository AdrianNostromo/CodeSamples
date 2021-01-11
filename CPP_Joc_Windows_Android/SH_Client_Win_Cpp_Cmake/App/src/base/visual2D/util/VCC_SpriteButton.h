#pragma once

#include <base/assets/util/ManagedAssetPath.h>
#include "VCC_Touchable.h"

class VCC_SpriteButton : public VCC_Touchable {priv typedef VCC_Touchable super;pub dCtor(VCC_SpriteButton);
	pub static int TYPE;

    pub ManagedAssetPath* managedAtlasPath;
	pub std::string buttonAssetsPrefix;

	pub std::string* touchDownSoundConfig_id;
	pub float touchDownSound_volume;
	pub std::string* touchUpSoundConfig_id;
	pub float touchUpSound_volume;
	pub std::string* disabledTouchDownSoundConfig_id;
	pub float disabledTouchDownSound_volume;

	pub static VCC_SpriteButton* New(
		ManagedAssetPath* managedAtlasPath, std::string buttonAssetsPrefix,
		std::string* touchDownSoundConfig_id, float touchDownSound_volume,
		std::string* touchUpSoundConfig_id, float touchUpSound_volume,
		std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume,
		int touchShape);
	pub static VCC_SpriteButton* New(
		ManagedAssetPath* managedAtlasPath, std::string buttonAssetsPrefix,
		std::string* touchDownSoundConfig_id, float touchDownSound_volume,
		std::string* touchUpSoundConfig_id, float touchUpSound_volume,
		std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume);
	pub static VCC_SpriteButton* New(
		std::string atlasPath, std::string buttonAssetsPrefix,
		std::string* touchDownSoundConfig_id, float touchDownSound_volume,
		std::string* touchUpSoundConfig_id, float touchUpSound_volume,
		std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume,
		int touchShapeS);
	pub static VCC_SpriteButton* New(
		std::string atlasPath, std::string buttonAssetsPrefix,
		std::string* touchDownSoundConfig_id, float touchDownSound_volume,
		std::string* touchUpSoundConfig_id, float touchUpSound_volume,
		std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume);

	priv explicit VCC_SpriteButton(
		ManagedAssetPath* managedAtlasPath, std::string buttonAssetsPrefix,
		std::string* touchDownSoundConfig_id, float touchDownSound_volume,
		std::string* touchUpSoundConfig_id, float touchUpSound_volume,
		std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume, int touchShape);
	priv explicit VCC_SpriteButton(
		ManagedAssetPath* managedAtlasPath, std::string buttonAssetsPrefix,
		std::string* touchDownSoundConfig_id, float touchDownSound_volume,
		std::string* touchUpSoundConfig_id, float touchUpSound_volume,
		std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume);
	priv explicit VCC_SpriteButton(
		std::string atlasPath, std::string buttonAssetsPrefix,
		std::string* touchDownSoundConfig_id, float touchDownSound_volume,
		std::string* touchUpSoundConfig_id, float touchUpSound_volume,
		std::string* disabledTouchDownSoundConfig_id, float disabledTouchDownSound_volume,
		int touchShape);

	pub virtual ~VCC_SpriteButton();
};
