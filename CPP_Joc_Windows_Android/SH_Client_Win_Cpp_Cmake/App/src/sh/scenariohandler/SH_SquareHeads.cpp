#include <sh/app/IAppSquareHeads.h>
#include <base/exceptions/UnhandledCaseException.h>
#include "SH_SquareHeads.h"
#include <sh/scenarios/startarea_a_v1/gw/config/ScenarioFactory.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/ScenarioFactory.h>
#include <rpg3D/gw/ui/GPUIMainUIView.h>
#include <sh/scenarios/startarea_a_v1/gw/manager/GPManPlatforms.h>
#include <sh/scenarios/battlezones_a_v1/gw/manager/GPManPlatforms.h>
#include <worldGame3D/gw/handler/GWEventDataBase.h>
#include <base/util/StringUtil.h>
#include <worldGame3D/scripts/ScriptCommands.h>
#include <sh/scenariohandler/command/CommandsParser.h>
#include <sh/scenariohandler/command/ICommand.h>
#include <sh/viewsHandler/IViewsHandler.h>
#include <sh/view/mainsidebar/IMainSidebarView.h>

using namespace sh;

SH_SquareHeads::SH_SquareHeads(
    IRenderingHandler* renderingHandler,
    IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
    IContainer2D* layer2DGameWorld,
    IAppSquareHeads* appLocal)
    : super(renderingHandler, appAssets, appMetrics, appUtils, layer2DGameWorld, appLocal)
{
    //void
}

void SH_SquareHeads::onGWEvent(std::string id, std::shared_ptr<base::GWEventDataBase> data) {
    bool isProcessed = true;
    try {
        super::onGWEvent(id, data);
    }catch(UnhandledCaseException& x) {
        isProcessed = false;
    }
    if(isProcessed) {
        return ;
    }

	if (id == base::IGameWorld::GWE_RequestRunScript) {
		if (data == nullptr) {
			throw LogicException(LOC);
		}

		std::string* scriptString = data->paramSPS != nullptr ? data->paramSPS.get() : &data->paramS;
		if (scriptString->empty()) {
			throw LogicException(LOC);
		}

		using sc = worldGame3D::ScriptCommands;

		std::shared_ptr<ArrayList<std::string>> commandParts = StringUtil::split(*scriptString, sc::commandPartsDelimiter, true/*includeEmptyParts*/);
		if (commandParts->count() == 0) {
			// Empty command not allowed currently.
			throw LogicException(LOC);
		}

		int cPartIndex = 0;
		std::string* commandPart = commandParts->getPointer(cPartIndex++);
		if (*commandPart == sc::cmd_setScenarioAndZone) {
			std::string* scenarioId = nullptr;
			bool restartScenario = false;
			std::string* zoneName = nullptr;
			bool restartZone = false;
			
			std::string* commandPart = commandParts->getPointer(cPartIndex++);
			if (*commandPart == sc::sameScenario) {
				// Same scenario, only the zone changes probably.
				if (getActiveScenarioTemplate() == nullptr) {
					throw LogicException(LOC);
				}
				scenarioId = &getActiveScenarioTemplate()->id;
			} else {
				scenarioId = commandPart;
			}

			// The next parts after the scenarioId are optional.
			commandPart = commandParts->getPointer(cPartIndex++, nullptr);

			if (commandPart != nullptr  && *commandPart == sc::restart) {
				restartScenario = true;

				commandPart = commandParts->getPointer(cPartIndex++, nullptr);
			}

			if (commandPart != nullptr) {
				zoneName = commandPart;
				
				commandPart = commandParts->getPointer(cPartIndex++, nullptr);
			}
			if (commandPart != nullptr && *commandPart == sc::restart) {
				restartZone = true;

				commandPart = commandParts->getPointer(cPartIndex++, nullptr);
			}

			if (commandPart != nullptr) {
				// Unprocessed command parts occured.
				throw LogicException(LOC);
			}

			ScenarioConfig* scenarioConfig = nullptr;
			scenarioConfig = new ScenarioConfig(nullptr/*serverConnectConfig*/);

			int r = setActiveScenario(
				*scenarioId, scenarioConfig, restartScenario/*forceScenarioRestartIfSame*/,
				zoneName != nullptr ? *zoneName : ""/*optionalZoneName*/, restartZone/*forceZoneRestartIfSame*/
			);
			if (r != 0) {
				throw LogicException(LOC);
			}
		} else if (*commandPart == sc::cmd_setZone) {
			std::string* scenarioId = nullptr;
			bool restartScenario = false;
			std::string* zoneName = nullptr;
			bool restartZone = false;

			if (getActiveScenarioTemplate() == nullptr) {
				throw LogicException(LOC);
			}
			scenarioId = &getActiveScenarioTemplate()->id;

			std::string* commandPart = commandParts->getPointer(cPartIndex++);
			zoneName = commandPart;
			commandPart = commandParts->getPointer(cPartIndex++, nullptr);

			if (commandPart != nullptr && *commandPart == sc::restart) {
				restartZone = true;

				commandPart = commandParts->getPointer(cPartIndex++, nullptr);
			}

			if (commandPart != nullptr) {
				// Unprocessed command parts occured.
				throw LogicException(LOC);
			}

			ScenarioConfig* scenarioConfig = nullptr;
			scenarioConfig = new ScenarioConfig(nullptr/*serverConnectConfig*/);

			int r = setActiveScenario(
				*scenarioId, scenarioConfig, restartScenario/*forceScenarioRestartIfSame*/,
				zoneName != nullptr ? *zoneName : ""/*optionalZoneName*/, restartZone/*forceZoneRestartIfSame*/
			);
			if (r != 0) {
				throw LogicException(LOC);
			}
		} else {
			// Use the new system.

			// Start from the start.
			cPartIndex = 0;

			sp<ArrayList<sp<ICommand>>> commandsList = CommandsParser::ParseAllCommands(*commandParts);
			for (int i = 0; i < commandsList->count(); i++) {
				sp<ICommand> commandObject = commandsList->getDirect(i);

				commandObject->run(this, data->sourceTeleporterOffset);
			}
		}
	}
	
	else if (id == base::IGameWorld::GWE_RequestShowMainSidebarView) {
		appLocal->getComponentAs<sh::IViewsHandler*>(true/*mustExist*/)->getOrCreateMainSidebarView()->show(true);
	}
	
	else if(getActiveScenarioTemplate() == battlezones_a_v1::ScenarioFactory::scenarioTemplate) {
        if(id == battlezones_a_v1::GPManPlatforms::GWE_RequestStartScenario_StartArea) {
			int r = setActiveScenario(
				startarea_a_v1::ScenarioFactory::scenarioTemplate->id, new ScenarioConfig(nullptr/*serverConnectConfig*/), false/*forceScenarioRestartIfSame*/,
				""/*optionalZoneName*/, false/*forceZoneRestartIfSame*/
			);
			if (r != 0) {
				throw LogicException(LOC);
			}
        }else {
            throw UnhandledCaseException();
        }
    }
	
	else {
        throw UnhandledCaseException();
    }
}

SH_SquareHeads::~SH_SquareHeads() {
    //void
}
