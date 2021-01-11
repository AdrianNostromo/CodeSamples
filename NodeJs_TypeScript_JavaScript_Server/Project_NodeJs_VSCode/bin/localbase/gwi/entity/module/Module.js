"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ModuleSnapshotDelta_1 = require("./ModuleSnapshotDelta");
class Module extends ModuleSnapshotDelta_1.ModuleSnapshotDelta {
    constructor(moduleId, entity, usesGameLoop) {
        super(moduleId, entity, usesGameLoop);
        this.entity.selfAddModule(this);
    }
}
exports.Module = Module;
//# sourceMappingURL=Module.js.map