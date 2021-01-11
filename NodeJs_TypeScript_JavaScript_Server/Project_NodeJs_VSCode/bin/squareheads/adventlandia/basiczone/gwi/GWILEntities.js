"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWILBase_1 = require("./GWILBase");
class GWILEntities extends GWILBase_1.GWILBase {
    constructor(handler, config) {
        super(handler, config);
        this.nexusTeamIndexCounter = 0;
        //void
    }
    reserveNextNexusTeamIndex() {
        this.nexusTeamIndexCounter++;
        return this.nexusTeamIndexCounter;
    }
}
exports.GWILEntities = GWILEntities;
//# sourceMappingURL=GWILEntities.js.map