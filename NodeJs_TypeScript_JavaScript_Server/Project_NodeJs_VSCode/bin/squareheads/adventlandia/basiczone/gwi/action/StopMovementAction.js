"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ActionIds_1 = require("./ActionIds");
const BasicAction_1 = require("localbase/gwi/action/BasicAction");
class StopMovementAction extends BasicAction_1.BasicAction {
    constructor(type, entityId) {
        super(type);
        this.entityId = entityId;
    }
}
StopMovementAction.TYPE = ActionIds_1.ActionIds.stopMovement;
exports.StopMovementAction = StopMovementAction;
//# sourceMappingURL=StopMovementAction.js.map