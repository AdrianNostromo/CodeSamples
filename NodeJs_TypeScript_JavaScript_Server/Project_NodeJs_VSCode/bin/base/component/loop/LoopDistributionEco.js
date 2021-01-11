"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LoopDistributionGeneral_1 = require("./LoopDistributionGeneral");
const LoopListTypes_1 = require("./config/LoopListTypes");
class LoopDistributionEco extends LoopDistributionGeneral_1.LoopDistributionGeneral {
    constructor() {
        super();
        this.eco_deltaS = 0;
        this.eco_deltaMS = 0;
        //void
    }
    distributeLoop(deltaS, deltaMS) {
        super.distributeLoop(deltaS, deltaMS);
        this.eco_deltaS += deltaS;
        this.eco_deltaMS += deltaMS;
        if (LoopDistributionEco.ECO_FPS == -1 || this.eco_deltaS >= (1 / LoopDistributionEco.ECO_FPS)) {
            this.appLoopEco(this.eco_deltaS, this.eco_deltaMS);
            this.eco_deltaS = 0;
            this.eco_deltaMS = 0;
        }
    }
    appLoopEco(deltaS, deltaMS) {
        let listIndex = LoopListTypes_1.LoopListTypes.LOOP_INDEX_ECO;
        let rootLoopUserNode = this.rootComponent.getFloatingLoopUser(listIndex);
        if (rootLoopUserNode == null) {
            return;
        }
        let currentLoopUserNode = rootLoopUserNode;
        do {
            if (!currentLoopUserNode.getIsDisposed()) {
                currentLoopUserNode.appLoop_Eco(deltaS, deltaMS);
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
LoopDistributionEco.ECO_FPS = 2;
exports.LoopDistributionEco = LoopDistributionEco;
//# sourceMappingURL=LoopDistributionEco.js.map