import { ModuleLocal } from "../ModuleLocal";
import { VoxelInventoryTemplate } from "../../template/VoxelInventoryTemplate";
import { EntityTemplate } from "localbase/gwi/entity/template/EntityTemplate";
import { WorldEntityLocal } from "../../WorldEntityLocal";

export class VoxelInventoryModuleBase extends ModuleLocal {

  public readonly template: VoxelInventoryTemplate;

  protected readonly inventoryItems: EntityTemplate[] = [];
  
  public constructor(entity: WorldEntityLocal, template: VoxelInventoryTemplate) {
    super(template.moduleId, entity, false);

    this.template = template;
  }

  // Note. The returned list must not be edited.
  public peekInventoryItems(): EntityTemplate[] {
    return this.inventoryItems;
  }
  
}
