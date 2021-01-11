"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LoopDistributionEco_1 = require("./LoopDistributionEco");
const LoopListTypes_1 = require("./config/LoopListTypes");
class LoopDistributionGameWorld extends LoopDistributionEco_1.LoopDistributionEco {
    constructor() {
        super();
        this.gameWorld_deltaS = 0;
        this.gameWorld_deltaMS = 0;
        //void
    }
    distributeLoop(deltaS, deltaMS) {
        super.distributeLoop(deltaS, deltaMS);
        this.gameWorld_deltaS += deltaS;
        this.gameWorld_deltaMS += deltaMS;
        if (LoopDistributionGameWorld.GAME_WORLD_FPS == -1 || this.gameWorld_deltaS >= (1 / LoopDistributionGameWorld.GAME_WORLD_FPS)) {
            this.appLoopGameWorld(this.gameWorld_deltaS, this.gameWorld_deltaMS);
            this.gameWorld_deltaS = 0;
            this.gameWorld_deltaMS = 0;
        }
    }
    appLoopGameWorld(deltaS, deltaMS) {
        let listIndex = LoopListTypes_1.LoopListTypes.LOOP_INDEX_GAME_WORLD;
        let rootLoopUserNode = this.rootComponent.getFloatingLoopUser(listIndex);
        if (rootLoopUserNode == null) {
            return;
        }
        let currentLoopUserNode = rootLoopUserNode;
        do {
            if (!currentLoopUserNode.getIsDisposed()) {
                currentLoopUserNode.appLoop_GameWorld(deltaS, deltaMS);
            }
            let nextLoopUserNode = currentLoopUserNode.getNextLoopUserNode(listIndex);
            if (currentLoopUserNode.getIsDisposed()) {
                currentLoopUserNode.removeLoopUserNode(listIndex);
                // If removing the root node, save the next one as root.
                // This is required for list end detection.
                if (currentLoopUserNode == rootLoopUserNode) {
                    rootLoopUserNode = nextLoopUserNode;
                }
            }
            currentLoopUserNode = nextLoopUserNode;
        } while (currentLoopUserNode != rootLoopUserNode);
    }
}
// Must be float because it is used in float math (To avoid unwanted rounding).
// -1: max fps available.
LoopDistributionGameWorld.GAME_WORLD_FPS = 5;
exports.LoopDistributionGameWorld = LoopDistributionGameWorld;
//# sourceMappingURL=LoopDistributionGameWorld.js.map