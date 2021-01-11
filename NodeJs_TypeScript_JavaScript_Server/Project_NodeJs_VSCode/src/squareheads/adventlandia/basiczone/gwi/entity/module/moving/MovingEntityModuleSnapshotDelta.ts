import { MovingEntityModuleBase } from "./MovingEntityModuleBase";
import { Module } from "localbase/gwi/entity/module/Module";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { MovingEntityTemplate } from "../../template/MovingEntityTemplate";
import { BooleanUtil } from "base/util/BooleanUtil";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export class MovingEntityModuleSnapshotDelta extends MovingEntityModuleBase {

  public static readonly FLAGS_LOCAL_MOVING_ENTITY = class {
    // fo, i
    public static readonly move: number = 1 + Module.FLAGS.LAST_USED_BIT_INDEX;
    // fo, i
    public static readonly stopMovement: number = 2 + Module.FLAGS.LAST_USED_BIT_INDEX;
    // fo, i
    public static readonly turn: number = 3 + Module.FLAGS.LAST_USED_BIT_INDEX;
    
    public static readonly LAST_USED_BIT_INDEX: number = 3 + Module.FLAGS.LAST_USED_BIT_INDEX;
  };

  public constructor(entity: WorldEntityLocal, template: MovingEntityTemplate) {
    super(entity, template);

    //void
  }

  public computeByteSize(flagsBitMap: number): number {
    let len: number = super.computeByteSize(flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.move)) {
      len += 4 * 2;
    }
    if(BooleanUtil.is(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.stopMovement)) {
      //void
    }
    if(BooleanUtil.is(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.turn)) {
      len += 4;
    }

    return len;
  }

  public writeToBuffer(buff: StreamBuffer, flagsBitMap: number): void {
    super.writeToBuffer(buff, flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.move)) {
      buff.writeFloat(this.getDestMoveX());
      buff.writeFloat(this.getDestMoveY());
    }
    if(BooleanUtil.is(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.stopMovement)) {
      //void
    }
    if(BooleanUtil.is(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.turn)) {
      buff.writeFloat(this.getDdestTurnDZ());
    }
  }

  public computeFullSyncFlagsBitMap(_flagsBitMap: number): number {
    let flagsBitMap: number = super.computeFullSyncFlagsBitMap(_flagsBitMap);

    if(this.getIsMoveInProgress()) {
      flagsBitMap = BooleanUtil.set(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.move);
    }
    if(this.getIsMovementStopInProgress()) {
      flagsBitMap = BooleanUtil.set(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.stopMovement);
    }
    if(this.getIsTurnInProgress()) {
      flagsBitMap = BooleanUtil.set(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.turn);
    }

    return flagsBitMap;
  }

  public move(destMoveX: number, destMoveY: number): void {
    super.move(destMoveX, destMoveY);

    this.getEntity().setSnapshotDeltaFlag(MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.move);
  }

  public stopMovement(): void {
    super.stopMovement();

    this.getEntity().setSnapshotDeltaFlag(MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.stopMovement);
  }
  
  public turn(destTurnDZ: number): void {
    super.turn(destTurnDZ);

    this.getEntity().setSnapshotDeltaFlag(MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.turn);
  }
  
}
