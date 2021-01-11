import { VoxelInventoryModuleSnapshotDelta } from "./VoxelInventoryModuleSnapshotDelta";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { VoxelInventoryTemplate } from "../../template/VoxelInventoryTemplate";

export class VoxelInventoryModule extends VoxelInventoryModuleSnapshotDelta {

  public constructor(entity: WorldEntityLocal, template: VoxelInventoryTemplate) {
    super(entity, template);

    //void
  }
  
}
