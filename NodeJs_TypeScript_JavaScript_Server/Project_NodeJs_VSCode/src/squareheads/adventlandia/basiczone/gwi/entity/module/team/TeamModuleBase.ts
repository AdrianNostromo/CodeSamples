import { ModuleLocal } from "../ModuleLocal";
import { TeamTemplate } from "../../template/TeamTemplate";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { LogicError } from "base/exception/LogicError";
import { BasicWorldEntity } from "localbase/gwi/entity/BasicWorldEntity";

class JoinNexusTeamRequest {
  
  public targetNexusEntityId: number;

  public constructor(targetNexusEntityId: number) {
    this.targetNexusEntityId = targetNexusEntityId;
  }
  
}

class RC_JoinNexusTeam {
  public static readonly UNUSED: number = 0;
  public static readonly SUCCESS: number = 1;
  public static readonly FAIL: number = 2;
}

export class TeamModuleBase extends ModuleLocal {

  public readonly template: TeamTemplate;
  
  // -1; Not on any team.
  // >= 1; On a valid team.
  // Team 0 is reserved and not used.
  private teamIndex: number = -1;
  
  private activeRequest_buildNexusAndJoinTeam: JoinNexusTeamRequest = null;
  // Result codes are only synced in incremental syncs. They are used for client logic sync (eg: update inventory after building something).
  // Result codes for the client request.
  // 0; Not used.
  // 1: Success.
  // 2: Nexus entity not found.
  // 3: Nexus found but has an invalid team module or value.
  private rc_joinNexusTeam: number = 0;
  
  public constructor(entity: WorldEntityLocal, template: TeamTemplate) {
    super(template.moduleId, entity, true);

    this.template = template;
  }

  public getTeamIndex(): number {
    return this.teamIndex;
  }

  public setTeamIndex(teamIndex: number): void {
    if(this.teamIndex == teamIndex) {
      return ;
    }

    this.teamIndex = teamIndex;
    
    this.onCurrentTeamIndexChanged();
  }

  protected onCurrentTeamIndexChanged(): void {
    //void
  }

  protected getRc_joinNexusTeam(): number {
    return this.rc_joinNexusTeam;
  }

  private initRc_joinNexusTeam(rc: number): void {
    if(this.rc_joinNexusTeam == rc) {
      return ;
    }

    this.rc_joinNexusTeam = rc;

    this.onRcChange_joinNexusTeamResult();
  }

  protected onRcChange_joinNexusTeamResult(): void {
    //void
  }

  public joinNexusTeam(targetNexusEntityId: number): void {
    if(this.activeRequest_buildNexusAndJoinTeam != null) {
      throw new LogicError("Error.1.");
    }

    this.activeRequest_buildNexusAndJoinTeam = new JoinNexusTeamRequest(
      targetNexusEntityId
    );
  }

  public isRequestInProgress_joinNexusTeam(): boolean {
    if(this.activeRequest_buildNexusAndJoinTeam != null) {
      return true;
    }

    return false;
  }

  public gameLoop(deltaS: number, deltaMS: number): void {
    super.gameLoop(deltaS, deltaMS);

    if(this.activeRequest_buildNexusAndJoinTeam != null) {
      let targetNexusEntity: BasicWorldEntity = this.getEntity().handler.getEntityWithEntityId(
        this.activeRequest_buildNexusAndJoinTeam.targetNexusEntityId
      )
      if(targetNexusEntity == null) {
        this.initRc_joinNexusTeam(RC_JoinNexusTeam.FAIL);
      }else {
        let targetNexusEntity_team: TeamModuleBase = targetNexusEntity.getModule<TeamModuleBase>(TeamTemplate.MODULE_ID);
        if(targetNexusEntity_team == null || targetNexusEntity_team.getTeamIndex() < 1) {
          this.initRc_joinNexusTeam(RC_JoinNexusTeam.FAIL);
        }else {
          this.initRc_joinNexusTeam(RC_JoinNexusTeam.SUCCESS);
          this.setTeamIndex(targetNexusEntity_team.getTeamIndex());
        }
      }

      this.activeRequest_buildNexusAndJoinTeam = null;
    }
  }

  public syncFlagsCleared(): void {
    super.syncFlagsCleared();

    if(this.rc_joinNexusTeam != RC_JoinNexusTeam.UNUSED) {
      this.rc_joinNexusTeam = RC_JoinNexusTeam.UNUSED;
    }
  }
  
}
