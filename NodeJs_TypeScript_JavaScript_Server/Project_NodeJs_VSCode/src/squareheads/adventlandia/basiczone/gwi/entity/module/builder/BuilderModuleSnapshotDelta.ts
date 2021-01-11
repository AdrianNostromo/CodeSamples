import { BuilderModuleBase } from "./BuilderModuleBase";
import { LumberjackModule } from "../lumberjack/LumberjackModule";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { BuilderTemplate } from "../../template/BuilderTemplate";
import { BooleanUtil } from "base/util/BooleanUtil";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export class BuilderModuleSnapshotDelta extends BuilderModuleBase {

  public static readonly FLAGS_LOCAL_BUILDER = class {
    // incremental
    public static readonly buildNexusAndJoinTeamResultCode: number = 2 + LumberjackModule.FLAGS_LOCAL_LUMBERJACK.LAST_USED_BIT_INDEX;

    public static readonly LAST_USED_BIT_INDEX: number = 2 + LumberjackModule.FLAGS_LOCAL_LUMBERJACK.LAST_USED_BIT_INDEX;
  };

  public constructor(entity: WorldEntityLocal, template: BuilderTemplate) {
    super(entity, template);

    //void
  }

  protected onRcChange_buildNexusAndJoinTeamResult(): void {
    super.onRcChange_buildNexusAndJoinTeamResult();

    this.getEntity().setSnapshotDeltaFlag(BuilderModuleSnapshotDelta.FLAGS_LOCAL_BUILDER.buildNexusAndJoinTeamResultCode);
  }

  public computeByteSize(flagsBitMap: number): number {
    let len: number = super.computeByteSize(flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, BuilderModuleSnapshotDelta.FLAGS_LOCAL_BUILDER.buildNexusAndJoinTeamResultCode)) {
      //void
    }

    return len;
  }

  public writeToBuffer(buff: StreamBuffer, flagsBitMap: number): void {
    super.writeToBuffer(buff, flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, BuilderModuleSnapshotDelta.FLAGS_LOCAL_BUILDER.buildNexusAndJoinTeamResultCode)) {
      //void
    }
  }

  public computeFullSyncFlagsBitMap(_flagsBitMap: number): number {
    let flagsBitMap: number = super.computeFullSyncFlagsBitMap(_flagsBitMap);

    //void

    return flagsBitMap;
  }

}
