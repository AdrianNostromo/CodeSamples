"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const VoxelInventoryModuleBase_1 = require("./VoxelInventoryModuleBase");
const LogicError_1 = require("base/exception/LogicError");
class VoxelInventoryModuleStatistics extends VoxelInventoryModuleBase_1.VoxelInventoryModuleBase {
    constructor(entity, template) {
        super(entity, template);
        this.itemsCountMap = new Map();
        this.uniqueInventoryItemTypesList = [];
        //void
    }
    incrementItemCount(itemTemplate) {
        let val = 0;
        if (this.itemsCountMap.has(itemTemplate)) {
            val = this.itemsCountMap.get(itemTemplate);
        }
        this.itemsCountMap.set(itemTemplate, val + 1);
        if (val + 1 == 1 && this.uniqueInventoryItemTypesList.indexOf(itemTemplate) < 0) {
            this.uniqueInventoryItemTypesList.push(itemTemplate);
        }
    }
    decrementItemCount(itemTemplate) {
        if (!this.itemsCountMap.has(itemTemplate)) {
            throw new LogicError_1.LogicError("Error.1.");
        }
        let val = this.itemsCountMap.get(itemTemplate);
        if (val < 1) {
            throw new LogicError_1.LogicError("Error.1.");
        }
        this.itemsCountMap.set(itemTemplate, val - 1);
        if (val - 1 == 0) {
            let i = this.uniqueInventoryItemTypesList.indexOf(itemTemplate);
            this.uniqueInventoryItemTypesList.splice(i, 1);
        }
    }
    clearItemsCount() {
        this.itemsCountMap.clear();
        this.uniqueInventoryItemTypesList = [];
    }
    getItemsCountWhereType(itemTemplate) {
        if (this.itemsCountMap.has(itemTemplate)) {
            return this.itemsCountMap.get(itemTemplate);
        }
        return 0;
    }
    hasResourcesList(resourceCostsList) {
        for (let i = 0; i < resourceCostsList.length; i++) {
            let resCost = resourceCostsList[i];
            if (!this.hasResources(resCost.resourceTags, resCost.itemsCount)) {
                return false;
            }
        }
        return true;
    }
    hasResources(resourceTags, itemsCount) {
        let itemsCountInInventory = 0;
        for (let i = 0; i < this.uniqueInventoryItemTypesList.length; i++) {
            let itemTemplate = this.uniqueInventoryItemTypesList[i];
            if (itemTemplate.hasTagsList(resourceTags)) {
                let ct = this.getItemsCountWhereType(itemTemplate);
                itemsCountInInventory += ct;
            }
        }
        if (itemsCountInInventory >= itemsCount) {
            return true;
        }
        return false;
    }
    getUniqueInventoryItemTypesList() {
        return this.uniqueInventoryItemTypesList;
    }
}
exports.VoxelInventoryModuleStatistics = VoxelInventoryModuleStatistics;
//# sourceMappingURL=VoxelInventoryModuleStatistics.js.map