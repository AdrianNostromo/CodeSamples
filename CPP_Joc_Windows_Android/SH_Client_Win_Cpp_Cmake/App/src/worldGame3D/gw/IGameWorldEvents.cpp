#include "IGameWorldEvents.h"
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace worldGame3D;

base::GWEvent* IGameWorldEvents::GWEvent_createComponentsPost = newt base::GWEvent("GWEvent_createComponentsPost");
base::GWEvent* IGameWorldEvents::GWEvent_createContent = newt base::GWEvent("GWEvent_createContent");

base::GWEvent* IGameWorldEvents::GWEvent_isPlayEnabled_changed = newt base::GWEvent("GWEvent_isPlayEnabled_changed");

base::GWEvent* IGameWorldEvents::GWEvent_dispose_views = newt base::GWEvent("GWEvent_dispose_views");
base::GWEvent* IGameWorldEvents::GWEvent_saveData = newt base::GWEvent("GWEvent_saveData");
base::GWEvent* IGameWorldEvents::GWEvent_disposePre_contentA = newt base::GWEvent("GWEvent_disposePre_contentA");
base::GWEvent* IGameWorldEvents::GWEvent_zoneDeactivate_pre = newt base::GWEvent("GWEvent_zoneDeactivate_pre");

base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_netInput = newt base::GWEvent("GWEvent_gameLoop_netInput");
base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_sync = newt base::GWEvent("GWEvent_gameLoop_sync");

base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_localInput = newt base::GWEvent("GWEvent_gameLoop_localInput");
base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_localInputPost = newt base::GWEvent("GWEvent_gameLoop_localInputPost");

base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_worldLogic = newt base::GWEvent("GWEvent_gameLoop_worldLogic");

base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_contentCreation = newt base::GWEvent("GWEvent_gameLoop_contentCreation");

base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_aiLogic = newt base::GWEvent("GWEvent_gameLoop_aiLogic");

base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_actionsProcessingSimulation = newt base::GWEvent("GWEvent_gameLoop_actionsProcessingSimulation");
base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_prePhysicsSimulation = newt base::GWEvent("GWEvent_gameLoop_prePhysicsSimulation");
base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_physicsSimulation = newt base::GWEvent("GWEvent_gameLoop_physicsSimulation");
base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_simulation = newt base::GWEvent("GWEvent_gameLoop_simulation");
base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_simulationB = newt base::GWEvent("GWEvent_gameLoop_simulationB");
base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_simulationC = newt base::GWEvent("GWEvent_gameLoop_simulationC");

base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_entityInteractions = newt base::GWEvent("GWEvent_gameLoop_entityInteractions");
base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_entitiesRemoval = newt base::GWEvent("GWEvent_gameLoop_entitiesRemoval");

base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_visualsUpdate = newt base::GWEvent("GWEvent_gameLoop_visualsUpdate");
base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_frustumCulling3DUpdate = newt base::GWEvent("GWEvent_gameLoop_frustumCulling3DUpdate");
base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_visualsCacheUpdate = newt base::GWEvent("GWEvent_gameLoop_visualsCacheUpdate");

base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_camera = newt base::GWEvent("GWEvent_gameLoop_camera");

base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_actionsSending = newt base::GWEvent("GWEvent_gameLoop_actionsSending");

base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_ui = newt base::GWEvent("GWEvent_gameLoop_ui");

base::GWEvent* IGameWorldEvents::GWEvent_gameLoop_post = newt base::GWEvent("GWEvent_gameLoop_post");
