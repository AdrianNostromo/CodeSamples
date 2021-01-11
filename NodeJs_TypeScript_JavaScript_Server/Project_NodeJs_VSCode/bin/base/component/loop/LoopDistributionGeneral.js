"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LoopDistributionBase_1 = require("./LoopDistributionBase");
const LoopListTypes_1 = require("./config/LoopListTypes");
class LoopDistributionGeneral extends LoopDistributionBase_1.LoopDistributionBase {
    constructor() {
        super();
        this.general_deltaS = 0;
        this.general_deltaMS = 0;
        //void
    }
    distributeLoop(deltaS, deltaMS) {
        super.distributeLoop(deltaS, deltaMS);
        this.general_deltaS += deltaS;
        this.general_deltaMS += deltaMS;
        if (LoopDistributionGeneral.GENERAL_FPS == -1 || this.general_deltaS >= (1 / LoopDistributionGeneral.GENERAL_FPS)) {
            this.appLoopGeneral(this.general_deltaS, this.general_deltaMS);
            this.general_deltaS = 0;
            this.general_deltaMS = 0;
        }
    }
    appLoopGeneral(deltaS, deltaMS) {
        let listIndex = LoopListTypes_1.LoopListTypes.LOOP_INDEX_GENERAL;
        let rootLoopUserNode = this.rootComponent.getFloatingLoopUser(listIndex);
        if (rootLoopUserNode == null) {
            return;
        }
        let currentLoopUserNode = rootLoopUserNode;
        do {
            if (!currentLoopUserNode.getIsDisposed()) {
                currentLoopUserNode.appLoop_General(deltaS, deltaMS);
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
LoopDistributionGeneral.GENERAL_FPS = 10;
exports.LoopDistributionGeneral = LoopDistributionGeneral;
//# sourceMappingURL=LoopDistributionGeneral.js.map