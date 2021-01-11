"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ActionIds_1 = require("./ActionIds");
const BasicAction_1 = require("localbase/gwi/action/BasicAction");
class JoinNexusTeamByUnitAction extends BasicAction_1.BasicAction {
    constructor(type, entityId, targetNexusEntityId) {
        super(type);
        this.entityId = entityId;
        this.targetNexusEntityId = targetNexusEntityId;
    }
}
JoinNexusTeamByUnitAction.TYPE = ActionIds_1.ActionIds.joinNexusTeamByUnit;
exports.JoinNexusTeamByUnitAction = JoinNexusTeamByUnitAction;
//# sourceMappingURL=JoinNexusTeamByUnitAction.js.map