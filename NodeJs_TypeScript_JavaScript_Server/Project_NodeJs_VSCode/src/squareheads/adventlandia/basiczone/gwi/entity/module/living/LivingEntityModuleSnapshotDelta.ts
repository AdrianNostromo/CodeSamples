import { LivingEntityModuleBase } from "./LivingEntityModuleBase";
import { MovingEntityModule } from "../moving/MovingEntityModule";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { LivingEntityTemplate } from "../../template/LivingEntityTemplate";
import { BooleanUtil } from "base/util/BooleanUtil";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export class LivingEntityModuleSnapshotDelta extends LivingEntityModuleBase {

  public static readonly FLAGS_LOCAL_LIVING_ENTITY = class {
    // fm
    public static readonly currentHP: number = 1 + MovingEntityModule.FLAGS_LOCAL_MOVING_ENTITY.LAST_USED_BIT_INDEX;

    public static readonly LAST_USED_BIT_INDEX: number = 1 + MovingEntityModule.FLAGS_LOCAL_MOVING_ENTITY.LAST_USED_BIT_INDEX;
  };

  public constructor(entity: WorldEntityLocal, template: LivingEntityTemplate) {
    super(entity, template);

    //void
  }

  protected onCurrentHPChanged(): void {
    super.onCurrentHPChanged();

    // This value is not incrementaly synced.
    //this.getEntity().setSnapshotDeltaFlag(LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY.currentHP);
  }

  public computeByteSize(flagsBitMap: number): number {
    let len: number = super.computeByteSize(flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY.currentHP)) {
      len += 4;
    }

    return len;
  }

  public writeToBuffer(buff: StreamBuffer, flagsBitMap: number): void {
    super.writeToBuffer(buff, flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY.currentHP)) {
      buff.writeInt(this.getCurrentHP());
    }
  }

  public computeFullSyncFlagsBitMap(_flagsBitMap: number): number {
    let flagsBitMap: number = super.computeFullSyncFlagsBitMap(_flagsBitMap);

    flagsBitMap = BooleanUtil.set(flagsBitMap, LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY.currentHP);

    return flagsBitMap;
  }

}
