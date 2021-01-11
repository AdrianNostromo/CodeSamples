import { BasicModuleTemplate } from "localbase/gwi/entity/template/BasicModuleTemplate";

export class VoxelInventoryTemplate extends BasicModuleTemplate {
  
  public static readonly MODULE_ID: string = "voxelInventory";
  
  public constructor() {
    super(VoxelInventoryTemplate.MODULE_ID, true);
    
    //void
  }

}
