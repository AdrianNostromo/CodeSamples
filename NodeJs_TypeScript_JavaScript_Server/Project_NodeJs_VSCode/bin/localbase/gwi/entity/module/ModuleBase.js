"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class ModuleBase {
    constructor(moduleId, entity, usesGameLoop) {
        this.moduleId = moduleId;
        this.entity = entity;
        this.usesGameLoop = usesGameLoop;
    }
    gameLoop(deltaS, deltaMS) {
        //void
    }
}
exports.ModuleBase = ModuleBase;
//# sourceMappingURL=ModuleBase.js.map