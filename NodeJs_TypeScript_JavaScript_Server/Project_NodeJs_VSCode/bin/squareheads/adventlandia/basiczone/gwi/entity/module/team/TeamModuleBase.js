"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ModuleLocal_1 = require("../ModuleLocal");
const TeamTemplate_1 = require("../../template/TeamTemplate");
const LogicError_1 = require("base/exception/LogicError");
class JoinNexusTeamRequest {
    constructor(targetNexusEntityId) {
        this.targetNexusEntityId = targetNexusEntityId;
    }
}
class RC_JoinNexusTeam {
}
RC_JoinNexusTeam.UNUSED = 0;
RC_JoinNexusTeam.SUCCESS = 1;
RC_JoinNexusTeam.FAIL = 2;
class TeamModuleBase extends ModuleLocal_1.ModuleLocal {
    constructor(entity, template) {
        super(template.moduleId, entity, true);
        // -1; Not on any team.
        // >= 1; On a valid team.
        // Team 0 is reserved and not used.
        this.teamIndex = -1;
        this.activeRequest_buildNexusAndJoinTeam = null;
        // Result codes are only synced in incremental syncs. They are used for client logic sync (eg: update inventory after building something).
        // Result codes for the client request.
        // 0; Not used.
        // 1: Success.
        // 2: Nexus entity not found.
        // 3: Nexus found but has an invalid team module or value.
        this.rc_joinNexusTeam = 0;
        this.template = template;
    }
    getTeamIndex() {
        return this.teamIndex;
    }
    setTeamIndex(teamIndex) {
        if (this.teamIndex == teamIndex) {
            return;
        }
        this.teamIndex = teamIndex;
        this.onCurrentTeamIndexChanged();
    }
    onCurrentTeamIndexChanged() {
        //void
    }
    getRc_joinNexusTeam() {
        return this.rc_joinNexusTeam;
    }
    initRc_joinNexusTeam(rc) {
        if (this.rc_joinNexusTeam == rc) {
            return;
        }
        this.rc_joinNexusTeam = rc;
        this.onRcChange_joinNexusTeamResult();
    }
    onRcChange_joinNexusTeamResult() {
        //void
    }
    joinNexusTeam(targetNexusEntityId) {
        if (this.activeRequest_buildNexusAndJoinTeam != null) {
            throw new LogicError_1.LogicError("Error.1.");
        }
        this.activeRequest_buildNexusAndJoinTeam = new JoinNexusTeamRequest(targetNexusEntityId);
    }
    isRequestInProgress_joinNexusTeam() {
        if (this.activeRequest_buildNexusAndJoinTeam != null) {
            return true;
        }
        return false;
    }
    gameLoop(deltaS, deltaMS) {
        super.gameLoop(deltaS, deltaMS);
        if (this.activeRequest_buildNexusAndJoinTeam != null) {
            let targetNexusEntity = this.getEntity().handler.getEntityWithEntityId(this.activeRequest_buildNexusAndJoinTeam.targetNexusEntityId);
            if (targetNexusEntity == null) {
                this.initRc_joinNexusTeam(RC_JoinNexusTeam.FAIL);
            }
            else {
                let targetNexusEntity_team = targetNexusEntity.getModule(TeamTemplate_1.TeamTemplate.MODULE_ID);
                if (targetNexusEntity_team == null || targetNexusEntity_team.getTeamIndex() < 1) {
                    this.initRc_joinNexusTeam(RC_JoinNexusTeam.FAIL);
                }
                else {
                    this.initRc_joinNexusTeam(RC_JoinNexusTeam.SUCCESS);
                    this.setTeamIndex(targetNexusEntity_team.getTeamIndex());
                }
            }
            this.activeRequest_buildNexusAndJoinTeam = null;
        }
    }
    syncFlagsCleared() {
        super.syncFlagsCleared();
        if (this.rc_joinNexusTeam != RC_JoinNexusTeam.UNUSED) {
            this.rc_joinNexusTeam = RC_JoinNexusTeam.UNUSED;
        }
    }
}
exports.TeamModuleBase = TeamModuleBase;
//# sourceMappingURL=TeamModuleBase.js.map