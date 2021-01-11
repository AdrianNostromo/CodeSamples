#include "LD_Dispatcher.h"
#include <base/appLoop/config/LoopListTypes.h>
#include <base/exceptions/LogicException.h>
#include <TestLog.h>

using namespace base;

const float LD_Dispatcher::INPUT_DISPATCH_FPS = -1.0f;
const float LD_Dispatcher::GENERAL_FPS = -1.0f;
const float LD_Dispatcher::ECO_FPS = 10.0f;
const float LD_Dispatcher::GAME_WORLD_FPS = -1.0f;
const float LD_Dispatcher::UI_FPS = -1.0f;

LD_Dispatcher::LD_Dispatcher()
	: super()
{
	//void
}

void LD_Dispatcher::distributeLoop(float deltaS, int deltaMS) {
	inputDispatch_deltaS += deltaS;
	inputDispatch_deltaMS += deltaMS;
	if (INPUT_DISPATCH_FPS == -1 || inputDispatch_deltaS >= (1.0f / INPUT_DISPATCH_FPS)) {
		inputDispatch_appTimeS += inputDispatch_deltaS;

		dispatchEvent(*AppLoopEvent_input->setLoopData(inputDispatch_deltaS, inputDispatch_deltaMS, inputDispatch_appTimeS));
		dispatchEvent(*AppLoopEvent_inputDispatch->setLoopData(inputDispatch_deltaS, inputDispatch_deltaMS, inputDispatch_appTimeS));

		inputDispatch_deltaS = 0;
		inputDispatch_deltaMS = 0;
	}

	general_deltaS += deltaS;
	general_deltaMS += deltaMS;
	if (GENERAL_FPS == -1 || general_deltaS >= (1.0f / GENERAL_FPS)) {
		general_appTimeS += general_deltaS;

		dispatchEvent(*AppLoopEvent_general->setLoopData(general_deltaS, general_deltaMS, general_appTimeS));

		general_deltaS = 0;
		general_deltaMS = 0;
	}

	eco_deltaS += deltaS;
	eco_deltaMS += deltaMS;
	if (ECO_FPS == -1 || eco_deltaS >= (1.0f / ECO_FPS)) {
		eco_appTimeS += eco_deltaS;

		dispatchEvent(*AppLoopEvent_eco->setLoopData(eco_deltaS, eco_deltaMS, eco_appTimeS));

		eco_deltaS = 0;
		eco_deltaMS = 0;
	}

	gameWorld_deltaS += deltaS;
	gameWorld_deltaMS += deltaMS;
	if (GAME_WORLD_FPS == -1 || gameWorld_deltaS >= (1.0f / GAME_WORLD_FPS)) {
		gameWorld_appTimeS += gameWorld_deltaS;

		dispatchEvent(*AppLoopEvent_gameWorld_pre->setLoopData(gameWorld_deltaS, gameWorld_deltaMS, gameWorld_appTimeS));
		dispatchEvent(*AppLoopEvent_gameWorld->setLoopData(gameWorld_deltaS, gameWorld_deltaMS, gameWorld_appTimeS));

		gameWorld_deltaS = 0;
		gameWorld_deltaMS = 0;
	}

	uI_deltaS += deltaS;
	uI_deltaMS += deltaMS;
	if (UI_FPS == -1 || uI_deltaS >= (1.0f / UI_FPS)) {
		uI_appTimeS += uI_deltaS;

		dispatchEvent(*AppLoopEvent_ui->setLoopData(uI_deltaS, uI_deltaMS, uI_appTimeS));

		uI_deltaS = 0;
		uI_deltaMS = 0;
	}
	
	// This event is currently used only to delayed dispose ui_views from events that the views dispatch.
	dispatchEvent(*AppLoopEvent_delayedComponentsDispose->setLoopData(uI_deltaS, uI_deltaMS, uI_appTimeS));
}

LD_Dispatcher::~LD_Dispatcher() {
	//void
}
