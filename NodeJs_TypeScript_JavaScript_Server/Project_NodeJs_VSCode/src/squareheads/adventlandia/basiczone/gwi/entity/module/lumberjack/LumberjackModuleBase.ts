import { ModuleLocal } from "../ModuleLocal";
import { LumberjackTemplate } from "../../template/LumberjackTemplate";
import { TreeChopRequest } from "./util/TreeChopRequest";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { Pos3D } from "base/util/Pos3D";
import { EntityTemplate } from "localbase/gwi/entity/template/EntityTemplate";
import { VoxelInventoryModule } from "../voxelinventory/VoxelInventoryModule";
import { VoxelInventoryTemplate } from "../../template/VoxelInventoryTemplate";

export class LumberjackModuleBase extends ModuleLocal {

  public readonly template: LumberjackTemplate;

  private activeChopRequests: TreeChopRequest[] = [];
  
  public constructor(entity: WorldEntityLocal, template: LumberjackTemplate) {
    super(template.moduleId, entity, true);

    this.template = template;
  }

  public throwChopAxe(treeEntityId: number, treePos: Pos3D, sortedDynamicVoxelIndex: number, collectedItemTemplate: EntityTemplate): void {
    // Save in a list and process instantly on the gameLoop, similar to the turn code.
    this.activeChopRequests.push(new TreeChopRequest(
      treeEntityId,
      treePos,
      sortedDynamicVoxelIndex,
      collectedItemTemplate
    ));
  }

  protected peekActiveChopRequests(): TreeChopRequest[] {
    return this.activeChopRequests;
  }

  public gameLoop(deltaS: number, deltaMS: number): void {
    super.gameLoop(deltaS, deltaMS);

    if(this.activeChopRequests.length > 0) {
      for(let i: number=0;i<this.activeChopRequests.length;i++) {
        let entry: TreeChopRequest = this.activeChopRequests[i];

        let targetTreeEntity: WorldEntityLocal = this.getEntity().handler.getEntityWithEntityId<WorldEntityLocal>(entry.treeEntityId);

        // Remove 1 hp from the target tree if it exists.
        if(targetTreeEntity != null) {
          targetTreeEntity.livingEntity.addCurrentHP(-1);
        }
        
        // Add the specified wood block to the player inventory.
        let voxelInventory: VoxelInventoryModule = this.getEntity().getModule<VoxelInventoryModule>(VoxelInventoryTemplate.MODULE_ID);
        voxelInventory.addItem(entry.collectedItemTemplate);
      }

      // The reset occurs after the clients sync finishes.
      //this.activeChopRequests = [];
    }
  }

  public syncFlagsCleared(): void {
    super.syncFlagsCleared();

    if(this.activeChopRequests.length > 0) {
      this.activeChopRequests = [];
    }
  }
  
}
