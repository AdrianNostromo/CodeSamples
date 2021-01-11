"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ActionIds_1 = require("./ActionIds");
const BasicAction_1 = require("localbase/gwi/action/BasicAction");
class TurnAction extends BasicAction_1.BasicAction {
    constructor(type, entityId, rotDZ) {
        super(type);
        this.entityId = entityId;
        this.rotDZ = rotDZ;
    }
}
TurnAction.TYPE = ActionIds_1.ActionIds.turn;
exports.TurnAction = TurnAction;
//# sourceMappingURL=TurnAction.js.map