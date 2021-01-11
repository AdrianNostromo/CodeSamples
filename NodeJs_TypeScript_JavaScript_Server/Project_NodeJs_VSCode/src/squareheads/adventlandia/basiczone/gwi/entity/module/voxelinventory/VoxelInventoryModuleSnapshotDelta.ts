import { VoxelInventoryModuleItemOperations } from "./VoxelInventoryModuleItemOperations";
import { LivingEntityModuleSnapshotDelta } from "../living/LivingEntityModuleSnapshotDelta";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { VoxelInventoryTemplate } from "../../template/VoxelInventoryTemplate";
import { BooleanUtil } from "base/util/BooleanUtil";
import { EntityTemplate } from "localbase/gwi/entity/template/EntityTemplate";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export class VoxelInventoryModuleSnapshotDelta extends VoxelInventoryModuleItemOperations {

  public static readonly FLAGS_LOCAL_VOXEL_INVENTORY = class {
    // full
    public static readonly voxelInventory: number = 1 + LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY.LAST_USED_BIT_INDEX;

    public static readonly LAST_USED_BIT_INDEX: number = 1 + LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY.LAST_USED_BIT_INDEX;
  };

  public constructor(entity: WorldEntityLocal, template: VoxelInventoryTemplate) {
    super(entity, template);

    //void
  }

  public computeByteSize(flagsBitMap: number): number {
    let len: number = super.computeByteSize(flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, VoxelInventoryModuleSnapshotDelta.FLAGS_LOCAL_VOXEL_INVENTORY.voxelInventory)) {
      // Get the number of item types in the inventory.
      let uniqueInventoryItemTypesList: EntityTemplate[] = this.getUniqueInventoryItemTypesList();
      
      // Get the number of bits per bitmap item.
      let bitmapEntryBitSize: number = Math.ceil(uniqueInventoryItemTypesList.length / 2);

      // Compute the len for:
      /// Number of unique item types.
      len += 4;//uniqueInventoryItemTypesList.length

      /// Unique item types strings.
      for(let i: number=0;i<uniqueInventoryItemTypesList.length;i++) {
        let uniqueItem: EntityTemplate = uniqueInventoryItemTypesList[i];
        
        len += NetMsgsBlock.stringInBufferSize(uniqueItem.id);
      }

      
      /// The size in bits of a entry i nthe bitmap.
      len += 4;//bitmapEntryBitSize
      
      /// Number of items.
      len += 4;//this.peekInventoryItems().length
      
      /// Inventory items bitMap.
      // Round the len to 1 byte (8 bits).
      let bitmapSizeBytes: number = Math.ceil((bitmapEntryBitSize * this.peekInventoryItems().length) / 8);
      len += bitmapSizeBytes;
    }

    return len;
  }

  public writeToBuffer(buff: StreamBuffer, flagsBitMap: number): void {
    super.writeToBuffer(buff, flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, VoxelInventoryModuleSnapshotDelta.FLAGS_LOCAL_VOXEL_INVENTORY.voxelInventory)) {
      // Get the number of item types in the inventory.
      let uniqueInventoryItemTypesList: EntityTemplate[] = this.getUniqueInventoryItemTypesList();
      
      // Get the number of bits per bitmap item.
      let bitmapEntryBitSize: number = Math.ceil(uniqueInventoryItemTypesList.length / 2);

      // Compute the len for:
      /// Number of unique item types.
      buff.writeInt(uniqueInventoryItemTypesList.length);

      /// Unique item types strings.
      for(let i: number=0;i<uniqueInventoryItemTypesList.length;i++) {
        let uniqueItem: EntityTemplate = uniqueInventoryItemTypesList[i];
        
        buff.writeString(uniqueItem.id);
      }

      /// The size in bits of a entry i nthe bitmap.
      buff.writeInt(bitmapEntryBitSize);

      /// Number of items.
      buff.writeInt(this.peekInventoryItems().length);
      
      /// Inventory items bitMap.
      // Round the len to 1 byte (8 bits).
      // Use a byte array bitmap and BooleanUtil logic.
      let bitmapSizeBytes: number = Math.ceil((bitmapEntryBitSize * this.peekInventoryItems().length) / 8);
      let invItemsBitMap: Buffer = Buffer.alloc(bitmapSizeBytes);
      let inventoryItems: EntityTemplate[] = this.peekInventoryItems();

      let currentByteIndex: number = 0;
      let currentBitIndex: number = 0;
      for(let i: number=0;i<inventoryItems.length;i++) {
        let invItem: EntityTemplate = inventoryItems[i];

        let itemBitValue: number = uniqueInventoryItemTypesList.indexOf(invItem);

        for(let j: number=0;j<bitmapEntryBitSize;j++) {
          // Write the bit.
          BooleanUtil.writeBitToBitMap(
            invItemsBitMap, currentByteIndex, currentBitIndex, 
            itemBitValue, j
          );

          // Increment the index.
          if(currentBitIndex + 1 >= 8) {
            currentByteIndex++;
            currentBitIndex = 0;
          }else {
            currentBitIndex++;
          }
        }
      }

      buff.writeBuffer(invItemsBitMap);
    }
  }

  public computeFullSyncFlagsBitMap(_flagsBitMap: number): number {
    let flagsBitMap: number = super.computeFullSyncFlagsBitMap(_flagsBitMap);

    // Update on full sync in case the player looses sync and requires a full re-sync. This will make sure he has a valid synced inventory value.
    flagsBitMap = BooleanUtil.set(flagsBitMap, VoxelInventoryModuleSnapshotDelta.FLAGS_LOCAL_VOXEL_INVENTORY.voxelInventory);

    return flagsBitMap;
  }

}
