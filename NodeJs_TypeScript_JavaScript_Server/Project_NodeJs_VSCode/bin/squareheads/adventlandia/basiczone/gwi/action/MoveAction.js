"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ActionIds_1 = require("./ActionIds");
const BasicAction_1 = require("localbase/gwi/action/BasicAction");
class MoveAction extends BasicAction_1.BasicAction {
    constructor(type, entityId, destX, destY) {
        super(type);
        this.entityId = entityId;
        this.destX = destX;
        this.destY = destY;
    }
}
MoveAction.TYPE = ActionIds_1.ActionIds.move;
exports.MoveAction = MoveAction;
//# sourceMappingURL=MoveAction.js.map