"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ModuleLocal_1 = require("../ModuleLocal");
const LogicError_1 = require("base/exception/LogicError");
class BuildNexusAndJoinTeamRequest {
    constructor(buildPos) {
        this.buildPos = buildPos;
    }
}
class RC_BuildNexusAndJoinTeam {
}
RC_BuildNexusAndJoinTeam.UNUSED = 0;
RC_BuildNexusAndJoinTeam.SUCCESS = 1;
RC_BuildNexusAndJoinTeam.FAIL = 2;
class BuilderModuleBase extends ModuleLocal_1.ModuleLocal {
    constructor(entity, template) {
        super(template.moduleId, entity, true);
        this.activeRequest_buildNexusAndJoinTeam = null;
        // Result codes are only synced in incremental syncs. They are used for client logic sync (eg: update inventory after building something).
        // 0; Not used.
        // 1; Success.
        // 2; Not enough blocks.
        this.rc_buildNexusAndJoinTeam = 0;
        this.template = template;
    }
    buildNexusAndJoinTeam(buildPos) {
        if (this.activeRequest_buildNexusAndJoinTeam != null) {
            throw new LogicError_1.LogicError("Error.1.");
        }
        this.activeRequest_buildNexusAndJoinTeam = new BuildNexusAndJoinTeamRequest(buildPos);
    }
    isRequestInProgress_buildNexusAndJoinTeam() {
        if (this.activeRequest_buildNexusAndJoinTeam != null) {
            return true;
        }
        return false;
    }
    getRc_buildNexusAndJoinTeam() {
        return this.rc_buildNexusAndJoinTeam;
    }
    initRc_buildNexusAndJoinTeam(rc) {
        if (this.rc_buildNexusAndJoinTeam == rc) {
            return;
        }
        this.rc_buildNexusAndJoinTeam = rc;
        this.onRcChange_buildNexusAndJoinTeamResult();
    }
    onRcChange_buildNexusAndJoinTeamResult() {
        //void
    }
    gameLoop(deltaS, deltaMS) {
        super.gameLoop(deltaS, deltaMS);
        if (this.activeRequest_buildNexusAndJoinTeam != null) {
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
    syncFlagsCleared() {
        super.syncFlagsCleared();
        if (this.rc_buildNexusAndJoinTeam != RC_BuildNexusAndJoinTeam.UNUSED) {
            this.rc_buildNexusAndJoinTeam = RC_BuildNexusAndJoinTeam.UNUSED;
        }
    }
}
exports.BuilderModuleBase = BuilderModuleBase;
//# sourceMappingURL=BuilderModuleBase.js.map