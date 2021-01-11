import { VoxelInventoryModuleStatistics } from "./VoxelInventoryModuleStatistics";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { VoxelInventoryTemplate } from "../../template/VoxelInventoryTemplate";
import { EntityTemplate } from "localbase/gwi/entity/template/EntityTemplate";

export class VoxelInventoryModuleItemOperations extends VoxelInventoryModuleStatistics {

  public constructor(entity: WorldEntityLocal, template: VoxelInventoryTemplate) {
    super(entity, template);

    //void
  }

  public addItem(itemTemplate: EntityTemplate): void {
    this.inventoryItems.push(itemTemplate);
    this.incrementItemCount(itemTemplate);
  }

  public addItemsList(itemTempatesList: EntityTemplate[]): void {
    for(let i: number=0;i<itemTempatesList.length;i++) {
        let itemTemplate: EntityTemplate = itemTempatesList[i];
        
        this.inventoryItems.push(itemTemplate);
        this.incrementItemCount(itemTemplate);
    }
  }

  public removeAllItems(): void {
    this.inventoryItems.splice(0, this.inventoryItems.length); 
    this.clearItemsCount();
  }

}
