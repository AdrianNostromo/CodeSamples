"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ZonesListManagerBase_1 = require("./ZonesListManagerBase");
const GWIExtenablePersistentIsolatedInstance_1 = require("localbase/gwi/GWIExtenablePersistentIsolatedInstance");
class ZonesListManagerList extends ZonesListManagerBase_1.ZonesListManagerBase {
    constructor(app) {
        super(app);
        this._ILoopUserBase_General = true;
        this.scenarioServersList = [];
        this.activeServersCount = 0;
        //void
    }
    onNetError(target) {
        // Show a log.
        this.getApp().getLog().debug('Scenario server. Net error occured.');
        // Dispose the match.
        this.removeScenarioServer(target);
    }
    onLogicError(target) {
        // Show a log.
        this.getApp().getLog().debug('Scenario server. Logic error occured.');
        // Dispose the match.
        this.removeScenarioServer(target);
    }
    getClientDevicesManager() {
        return this.getApp().getSquareHeads().getClientDevicesManager();
    }
    getClientSessionsManager() {
        return this.getApp().getSquareHeads().getClientSessionsManager();
    }
    getLog() {
        return this.getApp().getLog();
    }
    getOrCreateOptimallyJoinableScearioServer() {
        let optimalMatch = null;
        let optimalMatchFreeSlots;
        let initialisingMatch = null;
        // Optimisation: each time a match is sent to the player, increment a possibleNewPLayersCount with a timout of 10 seconds.
        // This will handle overcrouding.
        for (let i = this.scenarioServersList.length - 1; i >= 0; i--) {
            if (!(this.scenarioServersList[i] instanceof GWIExtenablePersistentIsolatedInstance_1.GWIExtenablePersistentIsolatedInstance)) {
                continue;
            }
            let match = this.scenarioServersList[i];
            if (match != null) {
                if (match.getIsInitFinished) {
                    if ((match.getPlayersCount() + match.getPossibleNewPlayersCount()) < ZonesListManagerList.OPTIMAL_MATCH_PLAYERS) {
                        let playersDiff = ZonesListManagerList.OPTIMAL_MATCH_PLAYERS - (match.getPlayersCount() + match.getPossibleNewPlayersCount());
                        if (optimalMatch == null || playersDiff < optimalMatchFreeSlots) {
                            optimalMatch = match;
                        }
                    }
                }
                else {
                    if (initialisingMatch == null) {
                        initialisingMatch = match;
                    }
                }
            }
        }
        if (optimalMatch == null && initialisingMatch != null) {
            optimalMatch = initialisingMatch;
        }
        return optimalMatch;
    }
    appLoop_General(deltaS, deltaMS) {
        this.appLoop_local(deltaS, deltaMS);
    }
    appLoop_local(deltaS, deltaMS) {
        if (!this.getApp().getComponentStatus().isOnline) {
            return;
        }
        for (let i = this.scenarioServersList.length - 1; i >= 0; i--) {
            let match = this.scenarioServersList[i];
            if (match != null) {
                if (match.isAnyPlayerConnected()) {
                    match.getHandlerData().iddleTimeS = 0;
                }
                else {
                    match.getHandlerData().iddleTimeS += deltaS;
                    if (match.getHandlerData().iddleTimeS >= ZonesListManagerList.IDDLE_SERVER_REMOVAL_THRESHOLD_S) {
                        this.getApp().getLog().debug('Removing iddle scenario server.');
                        this.removeScenarioServer(match);
                        continue;
                    }
                }
            }
        }
    }
    getActiveServersCount() {
        return this.activeServersCount;
    }
    removeScenarioServer(gameMatch) {
        this.getApp().getLog().debug('Match instance removed with id: ' + gameMatch.getServerInstanceId());
        let index = this.scenarioServersList.indexOf(gameMatch);
        if (index < 0) {
            this.getApp().getLog().error('ServerComponentValidator. Error.');
            this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
            return;
        }
        this.onScenarioServerRemoved_pre(gameMatch);
        this.disposeComponent(gameMatch);
        this.scenarioServersList.splice(index, 1);
    }
    onScenarioServerAdded(scenarioServer) {
        this.activeServersCount++;
        this.getApp().getLog().debug("Scenario server created. Id: " + scenarioServer.getServerInstanceId() + ". Total scenario servers count: " + this.activeServersCount);
    }
    onScenarioServerRemoved_pre(scenarioServer) {
        this.activeServersCount--;
        this.getApp().getLog().debug("Scenario server removed. Id: " + scenarioServer.getServerInstanceId() + ". Total scenario servers count: " + this.activeServersCount);
    }
    gameWorldError(target, err) {
        this.getApp().getLog().error('Game match error occured.');
        this.removeScenarioServer(target);
    }
}
ZonesListManagerList.IDDLE_SERVER_REMOVAL_THRESHOLD_S = 15;
// The optimal number of players per match.
// The server limit should be around +20.
ZonesListManagerList.OPTIMAL_MATCH_PLAYERS = 10;
exports.ZonesListManagerList = ZonesListManagerList;
//# sourceMappingURL=ZonesListManagerList.js.map