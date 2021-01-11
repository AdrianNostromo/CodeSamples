"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var _a;
"use strict";
const VoxelInventoryModuleItemOperations_1 = require("./VoxelInventoryModuleItemOperations");
const LivingEntityModuleSnapshotDelta_1 = require("../living/LivingEntityModuleSnapshotDelta");
const BooleanUtil_1 = require("base/util/BooleanUtil");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
class VoxelInventoryModuleSnapshotDelta extends VoxelInventoryModuleItemOperations_1.VoxelInventoryModuleItemOperations {
    constructor(entity, template) {
        super(entity, template);
        //void
    }
    computeByteSize(flagsBitMap) {
        let len = super.computeByteSize(flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, VoxelInventoryModuleSnapshotDelta.FLAGS_LOCAL_VOXEL_INVENTORY.voxelInventory)) {
            // Get the number of item types in the inventory.
            let uniqueInventoryItemTypesList = this.getUniqueInventoryItemTypesList();
            // Get the number of bits per bitmap item.
            let bitmapEntryBitSize = Math.ceil(uniqueInventoryItemTypesList.length / 2);
            // Compute the len for:
            /// Number of unique item types.
            len += 4; //uniqueInventoryItemTypesList.length
            /// Unique item types strings.
            for (let i = 0; i < uniqueInventoryItemTypesList.length; i++) {
                let uniqueItem = uniqueInventoryItemTypesList[i];
                len += NetMsgsBlock_1.NetMsgsBlock.stringInBufferSize(uniqueItem.id);
            }
            /// The size in bits of a entry i nthe bitmap.
            len += 4; //bitmapEntryBitSize
            /// Number of items.
            len += 4; //this.peekInventoryItems().length
            /// Inventory items bitMap.
            // Round the len to 1 byte (8 bits).
            let bitmapSizeBytes = Math.ceil((bitmapEntryBitSize * this.peekInventoryItems().length) / 8);
            len += bitmapSizeBytes;
        }
        return len;
    }
    writeToBuffer(buff, flagsBitMap) {
        super.writeToBuffer(buff, flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, VoxelInventoryModuleSnapshotDelta.FLAGS_LOCAL_VOXEL_INVENTORY.voxelInventory)) {
            // Get the number of item types in the inventory.
            let uniqueInventoryItemTypesList = this.getUniqueInventoryItemTypesList();
            // Get the number of bits per bitmap item.
            let bitmapEntryBitSize = Math.ceil(uniqueInventoryItemTypesList.length / 2);
            // Compute the len for:
            /// Number of unique item types.
            buff.writeInt(uniqueInventoryItemTypesList.length);
            /// Unique item types strings.
            for (let i = 0; i < uniqueInventoryItemTypesList.length; i++) {
                let uniqueItem = uniqueInventoryItemTypesList[i];
                buff.writeString(uniqueItem.id);
            }
            /// The size in bits of a entry i nthe bitmap.
            buff.writeInt(bitmapEntryBitSize);
            /// Number of items.
            buff.writeInt(this.peekInventoryItems().length);
            /// Inventory items bitMap.
            // Round the len to 1 byte (8 bits).
            // Use a byte array bitmap and BooleanUtil logic.
            let bitmapSizeBytes = Math.ceil((bitmapEntryBitSize * this.peekInventoryItems().length) / 8);
            let invItemsBitMap = Buffer.alloc(bitmapSizeBytes);
            let inventoryItems = this.peekInventoryItems();
            let currentByteIndex = 0;
            let currentBitIndex = 0;
            for (let i = 0; i < inventoryItems.length; i++) {
                let invItem = inventoryItems[i];
                let itemBitValue = uniqueInventoryItemTypesList.indexOf(invItem);
                for (let j = 0; j < bitmapEntryBitSize; j++) {
                    // Write the bit.
                    BooleanUtil_1.BooleanUtil.writeBitToBitMap(invItemsBitMap, currentByteIndex, currentBitIndex, itemBitValue, j);
                    // Increment the index.
                    if (currentBitIndex + 1 >= 8) {
                        currentByteIndex++;
                        currentBitIndex = 0;
                    }
                    else {
                        currentBitIndex++;
                    }
                }
            }
            buff.writeBuffer(invItemsBitMap);
        }
    }
    computeFullSyncFlagsBitMap(_flagsBitMap) {
        let flagsBitMap = super.computeFullSyncFlagsBitMap(_flagsBitMap);
        // Update on full sync in case the player looses sync and requires a full re-sync. This will make sure he has a valid synced inventory value.
        flagsBitMap = BooleanUtil_1.BooleanUtil.set(flagsBitMap, VoxelInventoryModuleSnapshotDelta.FLAGS_LOCAL_VOXEL_INVENTORY.voxelInventory);
        return flagsBitMap;
    }
}
VoxelInventoryModuleSnapshotDelta.FLAGS_LOCAL_VOXEL_INVENTORY = (_a = class {
    },
    // full
    _a.voxelInventory = 1 + LivingEntityModuleSnapshotDelta_1.LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY.LAST_USED_BIT_INDEX,
    _a.LAST_USED_BIT_INDEX = 1 + LivingEntityModuleSnapshotDelta_1.LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY.LAST_USED_BIT_INDEX,
    _a);
exports.VoxelInventoryModuleSnapshotDelta = VoxelInventoryModuleSnapshotDelta;
//# sourceMappingURL=VoxelInventoryModuleSnapshotDelta.js.map