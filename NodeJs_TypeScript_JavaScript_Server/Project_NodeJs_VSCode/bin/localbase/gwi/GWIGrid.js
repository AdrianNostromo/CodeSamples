"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWIMatchJoiner_1 = require("./GWIMatchJoiner");
class GWIGrid extends GWIMatchJoiner_1.GWIMatchJoiner {
    constructor(handler, config) {
        super(handler, config);
        //void
    }
    static meterToGrid(posM) {
        return Math.round(posM / GWIGrid.VOXEL_SIZE);
    }
    static gridToMeter(posG) {
        return posG * GWIGrid.VOXEL_SIZE;
    }
}
GWIGrid.VOXEL_SIZE = 0.1;
exports.GWIGrid = GWIGrid;
//# sourceMappingURL=GWIGrid.js.map