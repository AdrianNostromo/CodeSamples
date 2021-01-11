"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Module_1 = require("localbase/gwi/entity/module/Module");
class ModuleLocal extends Module_1.Module {
    constructor(moduleId, entity, usesGameLoop) {
        super(moduleId, entity, usesGameLoop);
        //void
    }
    getEntity() {
        return this.entity;
    }
}
exports.ModuleLocal = ModuleLocal;
//# sourceMappingURL=ModuleLocal.js.map