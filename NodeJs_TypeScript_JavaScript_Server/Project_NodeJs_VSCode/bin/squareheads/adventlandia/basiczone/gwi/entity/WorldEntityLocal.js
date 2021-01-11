"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicWorldEntity_1 = require("localbase/gwi/entity/BasicWorldEntity");
class WorldEntityLocal extends BasicWorldEntity_1.BasicWorldEntity {
    constructor(template, entityId, handler, gameWorldSnapshotDelta) {
        super(template, entityId, handler, gameWorldSnapshotDelta);
        this.movingEntity = null;
        this.livingEntity = null;
        this.lumberjack = null;
        this.voxelInventory = null;
        this.builder = null;
        this.team = null;
        //void
    }
    getHandler() {
        return this.handler;
    }
}
exports.WorldEntityLocal = WorldEntityLocal;
//# sourceMappingURL=WorldEntityLocal.js.map