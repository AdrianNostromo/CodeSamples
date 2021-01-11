"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const VoxelInventoryModuleStatistics_1 = require("./VoxelInventoryModuleStatistics");
class VoxelInventoryModuleItemOperations extends VoxelInventoryModuleStatistics_1.VoxelInventoryModuleStatistics {
    constructor(entity, template) {
        super(entity, template);
        //void
    }
    addItem(itemTemplate) {
        this.inventoryItems.push(itemTemplate);
        this.incrementItemCount(itemTemplate);
    }
    addItemsList(itemTempatesList) {
        for (let i = 0; i < itemTempatesList.length; i++) {
            let itemTemplate = itemTempatesList[i];
            this.inventoryItems.push(itemTemplate);
            this.incrementItemCount(itemTemplate);
        }
    }
    removeAllItems() {
        this.inventoryItems.splice(0, this.inventoryItems.length);
        this.clearItemsCount();
    }
}
exports.VoxelInventoryModuleItemOperations = VoxelInventoryModuleItemOperations;
//# sourceMappingURL=VoxelInventoryModuleItemOperations.js.map