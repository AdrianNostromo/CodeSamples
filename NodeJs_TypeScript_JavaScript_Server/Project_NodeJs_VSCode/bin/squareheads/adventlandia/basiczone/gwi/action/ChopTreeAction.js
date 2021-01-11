"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ActionIds_1 = require("./ActionIds");
const BasicAction_1 = require("localbase/gwi/action/BasicAction");
class ChopTreeAction extends BasicAction_1.BasicAction {
    constructor(type, entityId, treeEntityId, treePos, sortedDynamicVoxelIndex, collectedItemId) {
        super(type);
        this.entityId = entityId;
        this.treeEntityId = treeEntityId;
        this.treePos = treePos;
        this.sortedDynamicVoxelIndex = sortedDynamicVoxelIndex;
        this.collectedItemId = collectedItemId;
    }
}
ChopTreeAction.TYPE = ActionIds_1.ActionIds.chopTree;
exports.ChopTreeAction = ChopTreeAction;
//# sourceMappingURL=ChopTreeAction.js.map