#include "DeV_PageHardcodedEntries.h"
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/VisualPropertiesConfig.h>
#include <base/visual2D/util/VisualCreateConfig.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/util/StringUtil.h>
#include <base/opengl/util/GLUtil.h>
#include <base/fileSystem/fileHandler/util/FileIOProfilerStatics.h>
#include <TestLog.h>

using namespace base;

const int DeV_PageHardcodedEntries::DATA_LBLS_COUNT = 19;

int DeV_PageHardcodedEntries::DATA_LBL_INDEX_COUNTER = -1;
int DeV_PageHardcodedEntries::GetNextDataLblIndex() {
	DATA_LBL_INDEX_COUNTER++;

	return DATA_LBL_INDEX_COUNTER;
}

MenuItemConfig* DeV_PageHardcodedEntries::new_viewItemConfig_hardcodedEntries() {
	return new MenuItemConfig_ComposedVisual("hardcodedEntries", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("size", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_appSize.width")
			->put("height", 1.0f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("y",
				1.0f, "size.height",
				VisualValueConfig::OPERATION_ADD,
				-(5.0f + 56.0f), "_dpm.height"
			)

			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_in_._activePageIndex:0_", 1.0f, EaseConfig::ACCELERATION)
				->putState("_", 0.0f, EaseConfig::ACCELERATION)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_in_._activePageIndex:0_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("darkBg", "layer", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "size.width")
			->put("height", 1.0f, "size.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("width", 200.0f, "_dpm.width")
			->put("height", 20.0f * DATA_LBLS_COUNT + 2.0f, "_dpm.height")
			->put("x", 5.0f, "_dpm.width")
			->put("y", 0.0f, "_dpm.height")
			->put("tintColor", Color("#444444ff"))
			->put("alpha", 0.6f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblData" + std::to_string(GetNextDataLblIndex()), "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 1.0f)
			->put("x", 7.0f, "_dpm.width")
			->put("y", -5.0f - 20.0f * (DATA_LBL_INDEX_COUNTER + 1), "_dpm.height")
			->put("alpha", 0.8f)
			->put("tintColor", Color("#FFFFFFFF"))
		))
	);
}

DeV_PageHardcodedEntries::DeV_PageHardcodedEntries(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList)
	: super(app, viewItemConfigs, viewEaseDurationsSList)
{
	//void
}

void DeV_PageHardcodedEntries::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	// This is 
	cPageIndex = registerNewPage(false/*isDefaultPage*/, false/*usesDynamicEntries*/);
	if (cPageIndex != 0) {
		// This must be the first page because it is hardocded to be.
		throw LogicException(LOC);
	}

	for (int i = 0; ; i++) {
		BitmapLabel* lbl = getItemOptionalAs<BitmapLabel*>("hardcodedEntries.lblData" + std::to_string(i));
		if (lbl == nullptr) {
			break;
		}

		dataLblsArray.appendDirect(lbl);
	}
	if (DATA_LBLS_COUNT != dataLblsArray.count()) {
		// DATA_LBLS_COUNT is not set to the correct value.
		throw LogicException(LOC);
	}
}

void DeV_PageHardcodedEntries::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);

	if (isActive() && getActivePageIndex() == cPageIndex) {
		std::chrono::time_point<std::chrono::system_clock> tm = std::chrono::system_clock::now();

		if (!isTimersInit) {
			isTimersInit = true;

			s1_tmStart = tm;
			s1_tmLastFrame = tm;
			s1_framesCount = 0;

			s1_cpuTime_tmStart = clock();
		}

		s1_framesCount++;

		auto regionAutoDeltaTmMs = std::chrono::duration_cast<std::chrono::milliseconds>(tm - s1_tmStart);
		int regionDeltaTmMs = static_cast<int>(regionAutoDeltaTmMs.count());
		float regionDeltaTmS = regionDeltaTmMs / 1000.0f;

		int lblIndex = 0;

		frameIndex++;
		if (frameIndex > 1000000) {
			frameIndex = 0;
		}
		dataLblsArray.getDirect(lblIndex++)->setTextDirect("Frame: " + std::to_string(frameIndex));

		auto frameAutoDeltaTmMs = std::chrono::duration_cast<std::chrono::milliseconds>(tm - s1_tmLastFrame);
		int frameDeltaTmMs = static_cast<int>(frameAutoDeltaTmMs.count());

		if (s1_frameDurationMs_min == -1 || frameDeltaTmMs < s1_frameDurationMs_min) {
			s1_frameDurationMs_min = frameDeltaTmMs;
		}
		if (s1_frameDurationMs_max == -1 || frameDeltaTmMs > s1_frameDurationMs_max) {
			s1_frameDurationMs_max = frameDeltaTmMs;
		}

		s1_tmLastFrame = tm;

		s1_glCounter_2D_bindBuffer_count += GLUtil::gl->getProfiler2D()->bindBuffer;
		if (s1_glCounter_2D_bindBuffer_frameMin == -1 || GLUtil::gl->getProfiler2D()->bindBuffer < s1_glCounter_2D_bindBuffer_frameMin) {
			s1_glCounter_2D_bindBuffer_frameMin = GLUtil::gl->getProfiler2D()->bindBuffer;
		}
		if (s1_glCounter_2D_bindBuffer_frameMax == -1 || GLUtil::gl->getProfiler2D()->bindBuffer > s1_glCounter_2D_bindBuffer_frameMax) {
			s1_glCounter_2D_bindBuffer_frameMax = GLUtil::gl->getProfiler2D()->bindBuffer;
		}

		s1_glCounter_2D_bindTexture_count += GLUtil::gl->getProfiler2D()->bindTexture;
		if (s1_glCounter_2D_bindTexture_frameMin == -1 || GLUtil::gl->getProfiler2D()->bindTexture < s1_glCounter_2D_bindTexture_frameMin) {
			s1_glCounter_2D_bindTexture_frameMin = GLUtil::gl->getProfiler2D()->bindTexture;
		}
		if (s1_glCounter_2D_bindTexture_frameMax == -1 || GLUtil::gl->getProfiler2D()->bindTexture > s1_glCounter_2D_bindTexture_frameMax) {
			s1_glCounter_2D_bindTexture_frameMax = GLUtil::gl->getProfiler2D()->bindTexture;
		}

		s1_glCounter_2D_useProgram_count += GLUtil::gl->getProfiler2D()->useProgram;
		if (s1_glCounter_2D_useProgram_frameMin == -1 || GLUtil::gl->getProfiler2D()->useProgram < s1_glCounter_2D_useProgram_frameMin) {
			s1_glCounter_2D_useProgram_frameMin = GLUtil::gl->getProfiler2D()->useProgram;
		}
		if (s1_glCounter_2D_useProgram_frameMax == -1 || GLUtil::gl->getProfiler2D()->useProgram > s1_glCounter_2D_useProgram_frameMax) {
			s1_glCounter_2D_useProgram_frameMax = GLUtil::gl->getProfiler2D()->useProgram;
		}

		s1_glCounter_2D_drawElements_count += GLUtil::gl->getProfiler2D()->drawElements;
		if (s1_glCounter_2D_drawElements_frameMin == -1 || GLUtil::gl->getProfiler2D()->drawElements < s1_glCounter_2D_drawElements_frameMin) {
			s1_glCounter_2D_drawElements_frameMin = GLUtil::gl->getProfiler2D()->drawElements;
		}
		if (s1_glCounter_2D_drawElements_frameMax == -1 || GLUtil::gl->getProfiler2D()->drawElements > s1_glCounter_2D_drawElements_frameMax) {
			s1_glCounter_2D_drawElements_frameMax = GLUtil::gl->getProfiler2D()->drawElements;
		}

		s1_glCounter_2D_drawElementsCount_count += GLUtil::gl->getProfiler2D()->drawElementsCount;
		if (s1_glCounter_2D_drawElementsCount_frameMin == -1 || GLUtil::gl->getProfiler2D()->drawElementsCount < s1_glCounter_2D_drawElementsCount_frameMin) {
			s1_glCounter_2D_drawElementsCount_frameMin = GLUtil::gl->getProfiler2D()->drawElementsCount;
		}
		if (s1_glCounter_2D_drawElementsCount_frameMax == -1 || GLUtil::gl->getProfiler2D()->drawElementsCount > s1_glCounter_2D_drawElementsCount_frameMax) {
			s1_glCounter_2D_drawElementsCount_frameMax = GLUtil::gl->getProfiler2D()->drawElementsCount;
		}

		s1_glCounter_3D_bindBuffer_count += GLUtil::gl->getProfiler3D()->bindBuffer;
		if (s1_glCounter_3D_bindBuffer_frameMin == -1 || GLUtil::gl->getProfiler3D()->bindBuffer < s1_glCounter_3D_bindBuffer_frameMin) {
			s1_glCounter_3D_bindBuffer_frameMin = GLUtil::gl->getProfiler3D()->bindBuffer;
		}
		if (s1_glCounter_3D_bindBuffer_frameMax == -1 || GLUtil::gl->getProfiler3D()->bindBuffer > s1_glCounter_3D_bindBuffer_frameMax) {
			s1_glCounter_3D_bindBuffer_frameMax = GLUtil::gl->getProfiler3D()->bindBuffer;
		}

		s1_glCounter_3D_bindTexture_count += GLUtil::gl->getProfiler3D()->bindTexture;
		if (s1_glCounter_3D_bindTexture_frameMin == -1 || GLUtil::gl->getProfiler3D()->bindTexture < s1_glCounter_3D_bindTexture_frameMin) {
			s1_glCounter_3D_bindTexture_frameMin = GLUtil::gl->getProfiler3D()->bindTexture;
		}
		if (s1_glCounter_3D_bindTexture_frameMax == -1 || GLUtil::gl->getProfiler3D()->bindTexture > s1_glCounter_3D_bindTexture_frameMax) {
			s1_glCounter_3D_bindTexture_frameMax = GLUtil::gl->getProfiler3D()->bindTexture;
		}

		s1_glCounter_3D_useProgram_count += GLUtil::gl->getProfiler3D()->useProgram;
		if (s1_glCounter_3D_useProgram_frameMin == -1 || GLUtil::gl->getProfiler3D()->useProgram < s1_glCounter_3D_useProgram_frameMin) {
			s1_glCounter_3D_useProgram_frameMin = GLUtil::gl->getProfiler3D()->useProgram;
		}
		if (s1_glCounter_3D_useProgram_frameMax == -1 || GLUtil::gl->getProfiler3D()->useProgram > s1_glCounter_3D_useProgram_frameMax) {
			s1_glCounter_3D_useProgram_frameMax = GLUtil::gl->getProfiler3D()->useProgram;
		}

		s1_glCounter_3D_drawElements_count += GLUtil::gl->getProfiler3D()->drawElements;
		if (s1_glCounter_3D_drawElements_frameMin == -1 || GLUtil::gl->getProfiler3D()->drawElements < s1_glCounter_3D_drawElements_frameMin) {
			s1_glCounter_3D_drawElements_frameMin = GLUtil::gl->getProfiler3D()->drawElements;
		}
		if (s1_glCounter_3D_drawElements_frameMax == -1 || GLUtil::gl->getProfiler3D()->drawElements > s1_glCounter_3D_drawElements_frameMax) {
			s1_glCounter_3D_drawElements_frameMax = GLUtil::gl->getProfiler3D()->drawElements;
		}

		s1_glCounter_3D_drawElementsCount_count += GLUtil::gl->getProfiler3D()->drawElementsCount;
		if (s1_glCounter_3D_drawElementsCount_frameMin == -1 || GLUtil::gl->getProfiler3D()->drawElementsCount < s1_glCounter_3D_drawElementsCount_frameMin) {
			s1_glCounter_3D_drawElementsCount_frameMin = GLUtil::gl->getProfiler3D()->drawElementsCount;
		}
		if (s1_glCounter_3D_drawElementsCount_frameMax == -1 || GLUtil::gl->getProfiler3D()->drawElementsCount > s1_glCounter_3D_drawElementsCount_frameMax) {
			s1_glCounter_3D_drawElementsCount_frameMax = GLUtil::gl->getProfiler3D()->drawElementsCount;
		}

		if (regionDeltaTmS >= 1.0f) {
			float fps = s1_framesCount / regionDeltaTmS;
			float frameDurationMs_avg = regionDeltaTmMs / s1_framesCount;

			dataLblsArray.getDirect(lblIndex++)->setTextDirect("FPS: " + StringUtil::floatToFixedPrecisionString(fps, 2));
			dataLblsArray.getDirect(lblIndex++)->setTextDirect("Frame durations ms:");
			dataLblsArray.getDirect(lblIndex++)->setTextDirect(":" + StringUtil::floatToFixedPrecisionString(s1_frameDurationMs_min, 1) + " : " + StringUtil::floatToFixedPrecisionString(frameDurationMs_avg, 1) + " : " + StringUtil::floatToFixedPrecisionString(s1_frameDurationMs_max, 1));

			clock_t cpuTime = clock();
			clock_t cpuTimeDelta = cpuTime - s1_cpuTime_tmStart;
			float cpuTimeDeltaS = (float)cpuTimeDelta / CLOCKS_PER_SEC;
			float cpuTimeDeltaPercent = cpuTimeDeltaS / regionDeltaTmS;
			dataLblsArray.getDirect(lblIndex++)->setTextDirect("Cpu time: " + StringUtil::floatToFixedPrecisionString(cpuTimeDeltaS, 2) + "s : " + StringUtil::floatToFixedPrecisionString(cpuTimeDeltaPercent, 2) + "%");

			dataLblsArray.getDirect(lblIndex++)->setTextDirect("GL bindBuffer (min : avg : max : /1s):");
			dataLblsArray.getDirect(lblIndex++)->setTextDirect(
				":2D " + std::to_string(s1_glCounter_2D_bindBuffer_frameMin) + " : " + StringUtil::floatToFixedPrecisionString(s1_glCounter_2D_bindBuffer_count / s1_framesCount, 1) + " : " + std::to_string(s1_glCounter_2D_bindBuffer_frameMax) + " : " + std::to_string(s1_glCounter_2D_bindBuffer_count)
				+ " |3D " + std::to_string(s1_glCounter_3D_bindBuffer_frameMin) + " : " + StringUtil::floatToFixedPrecisionString(s1_glCounter_3D_bindBuffer_count / s1_framesCount, 1) + " : " + std::to_string(s1_glCounter_3D_bindBuffer_frameMax) + " : " + std::to_string(s1_glCounter_3D_bindBuffer_count)
			);

			dataLblsArray.getDirect(lblIndex++)->setTextDirect("GL bindTexture (min : avg : max : /1s):");
			dataLblsArray.getDirect(lblIndex++)->setTextDirect(
				":2D " + std::to_string(s1_glCounter_2D_bindTexture_frameMin) + " : " + StringUtil::floatToFixedPrecisionString(s1_glCounter_2D_bindTexture_count / s1_framesCount, 1) + " : " + std::to_string(s1_glCounter_2D_bindTexture_frameMax) + " : " + std::to_string(s1_glCounter_2D_bindTexture_count)
				+ " |3D " + std::to_string(s1_glCounter_3D_bindTexture_frameMin) + " : " + StringUtil::floatToFixedPrecisionString(s1_glCounter_3D_bindTexture_count / s1_framesCount, 1) + " : " + std::to_string(s1_glCounter_3D_bindTexture_frameMax) + " : " + std::to_string(s1_glCounter_3D_bindTexture_count)
			);

			dataLblsArray.getDirect(lblIndex++)->setTextDirect("GL useProgram (min : avg : max : /1s):");
			dataLblsArray.getDirect(lblIndex++)->setTextDirect(
				":2D " + std::to_string(s1_glCounter_2D_useProgram_frameMin) + " : " + StringUtil::floatToFixedPrecisionString(s1_glCounter_2D_useProgram_count / s1_framesCount, 1) + " : " + std::to_string(s1_glCounter_2D_useProgram_frameMax) + " : " + std::to_string(s1_glCounter_2D_useProgram_count)
				+ " |3D " + std::to_string(s1_glCounter_3D_useProgram_frameMin) + " : " + StringUtil::floatToFixedPrecisionString(s1_glCounter_3D_useProgram_count / s1_framesCount, 1) + " : " + std::to_string(s1_glCounter_3D_useProgram_frameMax) + " : " + std::to_string(s1_glCounter_3D_useProgram_count)
			);

			dataLblsArray.getDirect(lblIndex++)->setTextDirect("GL drawElements (min : avg : max : /1s):");
			dataLblsArray.getDirect(lblIndex++)->setTextDirect(
				":2D " + std::to_string(s1_glCounter_2D_drawElements_frameMin) + " : " + StringUtil::floatToFixedPrecisionString(s1_glCounter_2D_drawElements_count / s1_framesCount, 1) + " : " + std::to_string(s1_glCounter_2D_drawElements_frameMax) + " : " + std::to_string(s1_glCounter_2D_drawElements_count)
				+ " |3D " + std::to_string(s1_glCounter_3D_drawElements_frameMin) + " : " + StringUtil::floatToFixedPrecisionString(s1_glCounter_3D_drawElements_count / s1_framesCount, 1) + " : " + std::to_string(s1_glCounter_3D_drawElements_frameMax) + " : " + std::to_string(s1_glCounter_3D_drawElements_count)
			);

			dataLblsArray.getDirect(lblIndex++)->setTextDirect("GL drawElementsCount (min : avg : max : /1s):");
			dataLblsArray.getDirect(lblIndex++)->setTextDirect(
				":2D " + std::to_string(s1_glCounter_2D_drawElementsCount_frameMin) + " : " + StringUtil::floatToFixedPrecisionString(s1_glCounter_2D_drawElementsCount_count / s1_framesCount, 1) + " : " + std::to_string(s1_glCounter_2D_drawElementsCount_frameMax) + " : " + std::to_string(s1_glCounter_2D_drawElementsCount_count)
				+ " |3D " + std::to_string(s1_glCounter_3D_drawElementsCount_frameMin) + " : " + StringUtil::floatToFixedPrecisionString(s1_glCounter_3D_drawElementsCount_count / s1_framesCount, 1) + " : " + std::to_string(s1_glCounter_3D_drawElementsCount_frameMax) + " : " + std::to_string(s1_glCounter_3D_drawElementsCount_count)
			);

			dataLblsArray.getDirect(lblIndex++)->setTextDirect("File IO(sync total): " + std::to_string(FileIOProfilerStatics::COUNTER_FileIOSyncTotal));
			dataLblsArray.getDirect(lblIndex++)->setTextDirect("File IO(async total): " + std::to_string(FileIOProfilerStatics::COUNTER_FileIOAsyncTotal));
			
			{
				std::stringstream ss{};
				ss << "Loop ms times:";

				for (int i = 0; i < TestLog::a.timestampsArray.count(); i++) {
					std::chrono::time_point<std::chrono::system_clock> endTime = TestLog::a.timestampsArray.getDirect(i);
					auto chronoTimeDelta1Ms = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - TestLog::a.frameStartTimestamp);
					int timeDelta1Ms = static_cast<int>(chronoTimeDelta1Ms.count());
					
					ss << " " << timeDelta1Ms;
				}

				dataLblsArray.getDirect(lblIndex++)->setTextDirect(ss.str());
			}

			s1_tmStart = tm;
            s1_framesCount = 0;
            s1_frameDurationMs_min = -1.0f;
			s1_frameDurationMs_max = -1.0f;

			s1_cpuTime_tmStart = cpuTime;

			s1_glCounter_2D_bindBuffer_count = 0;
			s1_glCounter_2D_bindBuffer_frameMin = -1;
			s1_glCounter_2D_bindBuffer_frameMax = -1;

			s1_glCounter_2D_bindTexture_count = 0;
			s1_glCounter_2D_bindTexture_frameMin = -1;
			s1_glCounter_2D_bindTexture_frameMax = -1;

			s1_glCounter_2D_useProgram_count = 0;
			s1_glCounter_2D_useProgram_frameMin = -1;
			s1_glCounter_2D_useProgram_frameMax = -1;

			s1_glCounter_2D_drawElements_count = 0;
			s1_glCounter_2D_drawElements_frameMin = -1;
			s1_glCounter_2D_drawElements_frameMax = -1;

			s1_glCounter_2D_drawElementsCount_count = 0;
			s1_glCounter_2D_drawElementsCount_frameMin = -1;
			s1_glCounter_2D_drawElementsCount_frameMax = -1;

			s1_glCounter_3D_bindBuffer_count = 0;
			s1_glCounter_3D_bindBuffer_frameMin = -1;
			s1_glCounter_3D_bindBuffer_frameMax = -1;

			s1_glCounter_3D_bindTexture_count = 0;
			s1_glCounter_3D_bindTexture_frameMin = -1;
			s1_glCounter_3D_bindTexture_frameMax = -1;

			s1_glCounter_3D_useProgram_count = 0;
			s1_glCounter_3D_useProgram_frameMin = -1;
			s1_glCounter_3D_useProgram_frameMax = -1;

			s1_glCounter_3D_drawElements_count = 0;
			s1_glCounter_3D_drawElements_frameMin = -1;
			s1_glCounter_3D_drawElements_frameMax = -1;

			s1_glCounter_3D_drawElementsCount_count = 0;
			s1_glCounter_3D_drawElementsCount_frameMin = -1;
			s1_glCounter_3D_drawElementsCount_frameMax = -1;
		}
	}
}

DeV_PageHardcodedEntries::~DeV_PageHardcodedEntries() {
	//void
}
