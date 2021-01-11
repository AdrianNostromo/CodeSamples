"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ModuleLocal_1 = require("../ModuleLocal");
class VoxelInventoryModuleBase extends ModuleLocal_1.ModuleLocal {
    constructor(entity, template) {
        super(template.moduleId, entity, false);
        this.inventoryItems = [];
        this.template = template;
    }
    // Note. The returned list must not be edited.
    peekInventoryItems() {
        return this.inventoryItems;
    }
}
exports.VoxelInventoryModuleBase = VoxelInventoryModuleBase;
//# sourceMappingURL=VoxelInventoryModuleBase.js.map