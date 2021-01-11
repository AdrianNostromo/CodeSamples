"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ModuleLocal_1 = require("../ModuleLocal");
const TreeChopRequest_1 = require("./util/TreeChopRequest");
const VoxelInventoryTemplate_1 = require("../../template/VoxelInventoryTemplate");
class LumberjackModuleBase extends ModuleLocal_1.ModuleLocal {
    constructor(entity, template) {
        super(template.moduleId, entity, true);
        this.activeChopRequests = [];
        this.template = template;
    }
    throwChopAxe(treeEntityId, treePos, sortedDynamicVoxelIndex, collectedItemTemplate) {
        // Save in a list and process instantly on the gameLoop, similar to the turn code.
        this.activeChopRequests.push(new TreeChopRequest_1.TreeChopRequest(treeEntityId, treePos, sortedDynamicVoxelIndex, collectedItemTemplate));
    }
    peekActiveChopRequests() {
        return this.activeChopRequests;
    }
    gameLoop(deltaS, deltaMS) {
        super.gameLoop(deltaS, deltaMS);
        if (this.activeChopRequests.length > 0) {
            for (let i = 0; i < this.activeChopRequests.length; i++) {
                let entry = this.activeChopRequests[i];
                let targetTreeEntity = this.getEntity().handler.getEntityWithEntityId(entry.treeEntityId);
                // Remove 1 hp from the target tree if it exists.
                if (targetTreeEntity != null) {
                    targetTreeEntity.livingEntity.addCurrentHP(-1);
                }
                // Add the specified wood block to the player inventory.
                let voxelInventory = this.getEntity().getModule(VoxelInventoryTemplate_1.VoxelInventoryTemplate.MODULE_ID);
                voxelInventory.addItem(entry.collectedItemTemplate);
            }
            // The reset occurs after the clients sync finishes.
            //this.activeChopRequests = [];
        }
    }
    syncFlagsCleared() {
        super.syncFlagsCleared();
        if (this.activeChopRequests.length > 0) {
            this.activeChopRequests = [];
        }
    }
}
exports.LumberjackModuleBase = LumberjackModuleBase;
//# sourceMappingURL=LumberjackModuleBase.js.map