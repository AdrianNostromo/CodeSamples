import { VoxelInventoryModuleBase } from "./VoxelInventoryModuleBase";
import { EntityTemplate } from "localbase/gwi/entity/template/EntityTemplate";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { VoxelInventoryTemplate } from "../../template/VoxelInventoryTemplate";
import { LogicError } from "base/exception/LogicError";
import { ResourceCost } from "../../../util/ResourceCost";

export class VoxelInventoryModuleStatistics extends VoxelInventoryModuleBase {

  private readonly itemsCountMap: Map<EntityTemplate, number> = new Map<EntityTemplate, number>();

  private uniqueInventoryItemTypesList: EntityTemplate[] = [];

  public constructor(entity: WorldEntityLocal, template: VoxelInventoryTemplate) {
    super(entity, template);

    //void
  }

  protected incrementItemCount(itemTemplate: EntityTemplate): void {
    let val: number = 0;
    if(this.itemsCountMap.has(itemTemplate)) {
        val = this.itemsCountMap.get(itemTemplate);
    }
    
    this.itemsCountMap.set(itemTemplate, val + 1);
    if(val + 1 == 1 && this.uniqueInventoryItemTypesList.indexOf(itemTemplate) < 0) {
      this.uniqueInventoryItemTypesList.push(itemTemplate);
    }
  }
  
  protected decrementItemCount(itemTemplate: EntityTemplate): void {
    if(!this.itemsCountMap.has(itemTemplate)) {
      throw new LogicError("Error.1.");
    }
    
    let val: number = this.itemsCountMap.get(itemTemplate);
    if(val < 1) {
        throw new LogicError("Error.1.");
    }
    
    this.itemsCountMap.set(itemTemplate, val - 1);
    if(val - 1 == 0) {
      let i: number = this.uniqueInventoryItemTypesList.indexOf(itemTemplate);
      this.uniqueInventoryItemTypesList.splice(i, 1);
    }
  }
  
  protected clearItemsCount(): void {
      this.itemsCountMap.clear();
      this.uniqueInventoryItemTypesList = [];
  }
  
  public getItemsCountWhereType(itemTemplate: EntityTemplate): number {
    if(this.itemsCountMap.has(itemTemplate)) {
      return this.itemsCountMap.get(itemTemplate);
    }
    
    return 0;
}

  public hasResourcesList(resourceCostsList: ResourceCost[]): boolean {
      for(let i: number=0;i<resourceCostsList.length;i++) {
          let resCost: ResourceCost = resourceCostsList[i];
          
          if(!this.hasResources(resCost.resourceTags, resCost.itemsCount)) {
              return false;
          }
      }
      
      return true;
  }
  
  public hasResources(resourceTags: string[], itemsCount: number): boolean {
    let itemsCountInInventory: number = 0;
    
    for(let i: number=0;i<this.uniqueInventoryItemTypesList.length;i++) {
      let itemTemplate: EntityTemplate = this.uniqueInventoryItemTypesList[i];

      if(itemTemplate.hasTagsList(resourceTags)) {
        let ct: number = this.getItemsCountWhereType(itemTemplate);

        itemsCountInInventory += ct;
      }
    }
    if(itemsCountInInventory >= itemsCount) {
        return true;
    }
    
    return false;
  }

  protected getUniqueInventoryItemTypesList(): EntityTemplate[] {
    return this.uniqueInventoryItemTypesList;
  }

}
