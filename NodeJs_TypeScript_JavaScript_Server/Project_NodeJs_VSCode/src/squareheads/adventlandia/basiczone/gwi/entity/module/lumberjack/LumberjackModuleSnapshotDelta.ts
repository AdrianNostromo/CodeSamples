import { LumberjackModuleBase } from "./LumberjackModuleBase";
import { VoxelInventoryModuleSnapshotDelta } from "../voxelinventory/VoxelInventoryModuleSnapshotDelta";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { LumberjackTemplate } from "../../template/LumberjackTemplate";
import { Pos3D } from "base/util/Pos3D";
import { EntityTemplate } from "localbase/gwi/entity/template/EntityTemplate";
import { BooleanUtil } from "base/util/BooleanUtil";
import { TreeChopRequest } from "./util/TreeChopRequest";
import { LogicError } from "base/exception/LogicError";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export class LumberjackModuleSnapshotDelta extends LumberjackModuleBase {

  //asdzz;// refactr the flags system to a better method that works with local, not statics.
  public static readonly FLAGS_LOCAL_LUMBERJACK = class {
    // i
    public static readonly chopTreeList: number = 1 + VoxelInventoryModuleSnapshotDelta.FLAGS_LOCAL_VOXEL_INVENTORY.LAST_USED_BIT_INDEX;

    public static readonly LAST_USED_BIT_INDEX: number = 1 + VoxelInventoryModuleSnapshotDelta.FLAGS_LOCAL_VOXEL_INVENTORY.LAST_USED_BIT_INDEX;
  };

  public constructor(entity: WorldEntityLocal, template: LumberjackTemplate) {
    super(entity, template);

    //void
  }

  public throwChopAxe(treeEntityId: number, treePos: Pos3D, sortedDynamicVoxelIndex: number, collectedItemTemplate: EntityTemplate): void {
    super.throwChopAxe(treeEntityId, treePos, sortedDynamicVoxelIndex, collectedItemTemplate);

    this.getEntity().setSnapshotDeltaFlag(LumberjackModuleSnapshotDelta.FLAGS_LOCAL_LUMBERJACK.chopTreeList);
  }

  public computeByteSize(flagsBitMap: number): number {
    let len: number = super.computeByteSize(flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, LumberjackModuleSnapshotDelta.FLAGS_LOCAL_LUMBERJACK.chopTreeList)) {
      let activeChopRequests: TreeChopRequest[] = this.peekActiveChopRequests();
      if(activeChopRequests.length <= 0) {
        throw new LogicError("LumberjackModuleSnapshotDelta.computeByteSize.1.");
      }

      len += 4;//activeChopRequests.length

      for(let i: number=0;i<activeChopRequests.length;i++) {
        let entry: TreeChopRequest = activeChopRequests[i];
        
        len += entry.computeByteSize();
      }
    }

    return len;
  }

  public writeToBuffer(buff: StreamBuffer, flagsBitMap: number): void {
    super.writeToBuffer(buff, flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, LumberjackModuleSnapshotDelta.FLAGS_LOCAL_LUMBERJACK.chopTreeList)) {
      let activeChopRequests: TreeChopRequest[] = this.peekActiveChopRequests();
      if(activeChopRequests.length <= 0) {
        throw new LogicError("LumberjackModuleSnapshotDelta.writeToBuffer.1.");
      }

      buff.writeInt(activeChopRequests.length);

      for(let i: number=0;i<activeChopRequests.length;i++) {
        let entry: TreeChopRequest = activeChopRequests[i];
        
        entry.writeToBuffer(buff);
      }
    }
  }

  public computeFullSyncFlagsBitMap(_flagsBitMap: number): number {
    let flagsBitMap: number = super.computeFullSyncFlagsBitMap(_flagsBitMap);

    // NOTE : Because the tree chop is instant on the server, don't do a tree chop sync on the initial full sync.
    /// The full sync will include the player inventory with the chopped tree voxels.
    /*if(this.peekActiveChopRequests().length > 0) {
      flagsBitMap = BooleanUtil.set(flagsBitMap, LumberjackModuleSnapshotDelta.FLAGS_LOCAL_LUMBERJACK.chopTreeList);
    }*/

    return flagsBitMap;
  }

}
