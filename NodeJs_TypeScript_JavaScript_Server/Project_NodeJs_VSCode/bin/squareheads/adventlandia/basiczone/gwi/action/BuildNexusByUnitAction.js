"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ActionIds_1 = require("./ActionIds");
const BasicAction_1 = require("localbase/gwi/action/BasicAction");
class BuildNexusByUnitAction extends BasicAction_1.BasicAction {
    constructor(type, entityId, buildPos) {
        super(type);
        this.entityId = entityId;
        this.buildPos = buildPos;
    }
}
BuildNexusByUnitAction.TYPE = ActionIds_1.ActionIds.createNexusByUnit;
exports.BuildNexusByUnitAction = BuildNexusByUnitAction;
//# sourceMappingURL=BuildNexusByUnitAction.js.map