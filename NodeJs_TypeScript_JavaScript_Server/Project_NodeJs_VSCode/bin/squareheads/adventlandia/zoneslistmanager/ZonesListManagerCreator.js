"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ZonesListManagerList_1 = require("./ZonesListManagerList");
const DTGWIConfigBasicZone_1 = require("../basiczone/gwi/util/DTGWIConfigBasicZone");
const GameWorldInteractorLocal_1 = require("../basiczone/gwi/GameWorldInteractorLocal");
class ZonesListManagerCreator extends ZonesListManagerList_1.ZonesListManagerList {
    /*asdr
      private adventKingsshipServersMap: Map<number, AdventKingsshipGWI> = new Map();
    */
    constructor(app) {
        super(app);
        //void
    }
    getOrCreateOptimallyJoinableScearioServer() {
        let match = super.getOrCreateOptimallyJoinableScearioServer();
        if (match != null) {
            return match;
        }
        if (this.getActiveServersCount() >= ZonesListManagerCreator.MAX_MATCHES_COUNT) {
            return null;
        }
        // Create a new match instance if possible.
        match = this.createNewMatch();
        return match;
    }
    createNewMatch() {
        let serverInstanceId = ++ZonesListManagerCreator.gameMatchIdCounter;
        let config = new DTGWIConfigBasicZone_1.DTGWIConfigBasicZone(serverInstanceId, 20);
        let scenarioServer = new GameWorldInteractorLocal_1.GameWorldInteractorLocal(this, config);
        this.addComponent(scenarioServer);
        scenarioServer.setIsPlayEnabled(true);
        this.scenarioServersList.push(scenarioServer);
        this.onScenarioServerAdded(scenarioServer);
        return scenarioServer;
    }
}
ZonesListManagerCreator.MAX_MATCHES_COUNT = 10;
ZonesListManagerCreator.gameMatchIdCounter = 0;
ZonesListManagerCreator.serverInstanceIdCounter = 0;
exports.ZonesListManagerCreator = ZonesListManagerCreator;
//# sourceMappingURL=ZonesListManagerCreator.js.map