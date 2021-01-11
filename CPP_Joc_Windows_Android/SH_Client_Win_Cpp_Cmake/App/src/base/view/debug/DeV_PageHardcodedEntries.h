#pragma once

#include <base/gh.h>
#include "DeV_DynamicEntries.h"
#include <chrono>
#include <ctime>
#include <time.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>

namespace base {
class DeV_PageHardcodedEntries : public DeV_DynamicEntries {priv typedef DeV_DynamicEntries super;pub dCtor(DeV_PageHardcodedEntries);
	priv static const int DATA_LBLS_COUNT;

	priv static int DATA_LBL_INDEX_COUNTER;
	priv static int GetNextDataLblIndex();
	prot static MenuItemConfig* new_viewItemConfig_hardcodedEntries();

	priv int cPageIndex = -1;

	priv ArrayList<BitmapLabel*> dataLblsArray{};

	priv bool isTimersInit = false;

	priv int frameIndex = 0;

	priv clock_t s1_cpuTime_tmStart;

	// This will update once every 1 second.
	priv std::chrono::time_point<std::chrono::system_clock> s1_tmStart;
	priv std::chrono::time_point<std::chrono::system_clock> s1_tmLastFrame;
	// Use a float because this is used in division math logic.
	priv float s1_framesCount = 0;

	priv float s1_frameDurationMs_min = -1.0f;
	priv float s1_frameDurationMs_max = -1.0f;

	priv int s1_glCounter_2D_bindBuffer_count = 0;
	priv int s1_glCounter_2D_bindBuffer_frameMin = -1;
	priv int s1_glCounter_2D_bindBuffer_frameMax = -1;

	priv int s1_glCounter_2D_bindTexture_count = 0;
	priv int s1_glCounter_2D_bindTexture_frameMin = -1;
	priv int s1_glCounter_2D_bindTexture_frameMax = -1;
	
	priv int s1_glCounter_2D_useProgram_count = 0;
	priv int s1_glCounter_2D_useProgram_frameMin = -1;
	priv int s1_glCounter_2D_useProgram_frameMax = -1;
	
	priv int s1_glCounter_2D_drawElements_count = 0;
	priv int s1_glCounter_2D_drawElements_frameMin = -1;
	priv int s1_glCounter_2D_drawElements_frameMax = -1;
	
	priv int s1_glCounter_2D_drawElementsCount_count = 0;
	priv int s1_glCounter_2D_drawElementsCount_frameMin = -1;
	priv int s1_glCounter_2D_drawElementsCount_frameMax = -1;
	
	priv int s1_glCounter_3D_bindBuffer_count = 0;
	priv int s1_glCounter_3D_bindBuffer_frameMin = -1;
	priv int s1_glCounter_3D_bindBuffer_frameMax = -1;
	
	priv int s1_glCounter_3D_bindTexture_count = 0;
	priv int s1_glCounter_3D_bindTexture_frameMin = -1;
	priv int s1_glCounter_3D_bindTexture_frameMax = -1;
	
	priv int s1_glCounter_3D_useProgram_count = 0;
	priv int s1_glCounter_3D_useProgram_frameMin = -1;
	priv int s1_glCounter_3D_useProgram_frameMax = -1;
	
	priv int s1_glCounter_3D_drawElements_count = 0;
	priv int s1_glCounter_3D_drawElements_frameMin = -1;
	priv int s1_glCounter_3D_drawElements_frameMax = -1;
	
	priv int s1_glCounter_3D_drawElementsCount_count = 0;
	priv int s1_glCounter_3D_drawElementsCount_frameMin = -1;
	priv int s1_glCounter_3D_drawElementsCount_frameMax = -1;

    pub explicit DeV_PageHardcodedEntries(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList);
	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	pub void visualLoop(float deltaS, int deltaMS) override;

    pub ~DeV_PageHardcodedEntries() override;
};
};
