import { TeamModuleBase } from "./TeamModuleBase";
import { BuilderModule } from "../builder/BuilderModule";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { TeamTemplate } from "../../template/TeamTemplate";
import { BooleanUtil } from "base/util/BooleanUtil";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export class TeamModuleSnapshotDelta extends TeamModuleBase {

  public static readonly FLAGS_LOCAL_TEAM = class {
    // ful, incremental
    public static readonly teamIndex: number = 1 + BuilderModule.FLAGS_LOCAL_BUILDER.LAST_USED_BIT_INDEX;
    // incremental
    public static readonly joinNexusTeamResultCode: number = 2 + BuilderModule.FLAGS_LOCAL_BUILDER.LAST_USED_BIT_INDEX;

    public static readonly LAST_USED_BIT_INDEX: number = 2 + BuilderModule.FLAGS_LOCAL_BUILDER.LAST_USED_BIT_INDEX;
  };

  public constructor(entity: WorldEntityLocal, template: TeamTemplate) {
    super(entity, template);

    //void
  }

  protected onCurrentTeamIndexChanged(): void {
    super.onCurrentTeamIndexChanged();

    this.getEntity().setSnapshotDeltaFlag(TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.teamIndex);
  }

  protected onRcChange_joinNexusTeamResult(): void {
    super.onRcChange_joinNexusTeamResult();

    this.getEntity().setSnapshotDeltaFlag(TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.joinNexusTeamResultCode);
  }

  public computeByteSize(flagsBitMap: number): number {
    let len: number = super.computeByteSize(flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.teamIndex)) {
      len += 4;
    }
    if(BooleanUtil.is(flagsBitMap, TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.joinNexusTeamResultCode)) {
      len += 4;
    }

    return len;
  }

  public writeToBuffer(buff: StreamBuffer, flagsBitMap: number): void {
    super.writeToBuffer(buff, flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.teamIndex)) {
      buff.writeInt(this.getTeamIndex());
    }
    if(BooleanUtil.is(flagsBitMap, TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.joinNexusTeamResultCode)) {
      buff.writeInt(this.getRc_joinNexusTeam());
    }
  }

  public computeFullSyncFlagsBitMap(_flagsBitMap: number): number {
    let flagsBitMap: number = super.computeFullSyncFlagsBitMap(_flagsBitMap);

    flagsBitMap = BooleanUtil.set(flagsBitMap, TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.teamIndex);
    
    return flagsBitMap;
  }

}
