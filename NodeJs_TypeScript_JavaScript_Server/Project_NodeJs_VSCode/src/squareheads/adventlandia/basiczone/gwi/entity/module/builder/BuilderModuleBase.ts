import { Pos3D } from "base/util/Pos3D";
import { ModuleLocal } from "../ModuleLocal";
import { BuilderTemplate } from "../../template/BuilderTemplate";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { LogicError } from "base/exception/LogicError";
import { VoxelInventoryModule } from "../voxelinventory/VoxelInventoryModule";
import { EntityTemplatesLocal } from "../../../config/EntityTemplatesLocal";
import { exists } from "fs";

class BuildNexusAndJoinTeamRequest {
  
  public buildPos: Pos3D;

  public constructor(buildPos: Pos3D) {
    this.buildPos = buildPos;
  }

}

class RC_BuildNexusAndJoinTeam {
  public static readonly UNUSED: number = 0;
  public static readonly SUCCESS: number = 1;
  public static readonly FAIL: number = 2;
}

export class BuilderModuleBase extends ModuleLocal {

  public readonly template: BuilderTemplate;
  
  private activeRequest_buildNexusAndJoinTeam: BuildNexusAndJoinTeamRequest = null;

  // Result codes are only synced in incremental syncs. They are used for client logic sync (eg: update inventory after building something).
  // 0; Not used.
  // 1; Success.
  // 2; Not enough blocks.
  private rc_buildNexusAndJoinTeam: number = 0;

  public constructor(entity: WorldEntityLocal, template: BuilderTemplate) {
    super(template.moduleId, entity, true);

    this.template = template;
  }

  public buildNexusAndJoinTeam(buildPos: Pos3D): void {
    if(this.activeRequest_buildNexusAndJoinTeam != null) {
      throw new LogicError("Error.1.");
    }

    this.activeRequest_buildNexusAndJoinTeam = new BuildNexusAndJoinTeamRequest(
      buildPos
    );
  }

  public isRequestInProgress_buildNexusAndJoinTeam(): boolean {
    if(this.activeRequest_buildNexusAndJoinTeam != null) {
      return true;
    }

    return false;
  }

  protected getRc_buildNexusAndJoinTeam(): number {
    return this.rc_buildNexusAndJoinTeam;
  }

  private initRc_buildNexusAndJoinTeam(rc: number): void {
    if(this.rc_buildNexusAndJoinTeam == rc) {
      return ;
    }

    this.rc_buildNexusAndJoinTeam = rc;

    this.onRcChange_buildNexusAndJoinTeamResult();
  }

  protected onRcChange_buildNexusAndJoinTeamResult(): void {
    //void
  }

  public gameLoop(deltaS: number, deltaMS: number): void {
    super.gameLoop(deltaS, deltaMS);

    if(this.activeRequest_buildNexusAndJoinTeam != null) {
        //asdA;
        process.exit(-2);
    //   // Check if the player has enough resources.
    //   let voxelInventory: VoxelInventoryModule = this.getEntity().voxelInventory;
      
    //   EntityTemplatesLocal.BUILDING_NEXUS.asd;

    //   if(voxelInventory == null || !voxelInventory.containsResources(asd, asd)) {
    //     // Not enough resources to build that.
    //     this.initRc_buildNexusAndJoinTeam(RC_BuildNexusAndJoinTeam.FAIL);
    //   }else {
    //     // Check if the player is too close to other buildings.
    //     asd;

    //     // Get the next unused team index.
    //     this.getEntity().handler.getEntityWithEntityId
    //     asd++;
    //     let teamId: number = asd;
    
    //     // Build the nexus.
    //     asd;
    
    //     // Add the nexus to the new nexus team.
    //     asd;
    
    //     // Remove the required nexus build resources from the unti.
    //     asd;
    
    //     // Add the player to the nexus team.
    //     asd;
    //   }

      this.activeRequest_buildNexusAndJoinTeam = null;
    }
  }

  public syncFlagsCleared(): void {
    super.syncFlagsCleared();

    if(this.rc_buildNexusAndJoinTeam != RC_BuildNexusAndJoinTeam.UNUSED) {
      this.rc_buildNexusAndJoinTeam = RC_BuildNexusAndJoinTeam.UNUSED;
    }
  }
  
}
