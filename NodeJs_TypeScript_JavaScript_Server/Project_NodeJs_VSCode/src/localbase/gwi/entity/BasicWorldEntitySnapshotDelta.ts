import { BasicWorldEntityBase } from "./BasicWorldEntityBase";
import { EntityTemplate } from "./template/EntityTemplate";

import { GameWorldSnapshotDelta } from "./../sync/GameWorldSnapshotDelta";
import { BooleanUtil } from "base/util/BooleanUtil";
import { Module } from "./module/Module";
import { LogicError } from "base/exception/LogicError";
import { IEntityHandler } from "./util/IEntityHandler";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { GlobalAccessLogger } from "base/log/GlobalAccessLogger";

export class BasicWorldEntitySnapshotDelta extends BasicWorldEntityBase {

  public readonly gameWorldSnapshotDelta: GameWorldSnapshotDelta;
  
  protected flagsBitMap: number = 0;

  public static readonly FLAGS = class {
    // fm
    public static readonly ownerPlayerId: number = 0;
    // fm
    public static readonly pos: number = 1;
    // fm
    public static readonly rot: number = 2;
    // i
    public static readonly entityRemoved: number = 3;
    // fm
    public static readonly entityTemplateId: number = 4;

    public static readonly LAST_USED_BIT_INDEX: number = 4;
  };

  public isEntityRemoved: boolean = false;
  
  public constructor(template: EntityTemplate, entityId: number, handler: IEntityHandler, gameWorldSnapshotDelta: GameWorldSnapshotDelta) {
    super(template, entityId, handler);

    this.gameWorldSnapshotDelta = gameWorldSnapshotDelta;

    this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.ownerPlayerId);
    this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.pos);
    this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.rot);
    this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.entityTemplateId);
    
  }

  public clearFlags(): void {
    this.flagsBitMap = 0;

    for(let i:number=0;i<this.modulesList.length;i++) {
      let entry: Module = this.modulesList[i];
      entry.syncFlagsCleared();
    }
  }

  public setSnapshotDeltaFlag(bitMapFlag: number): void {
    if(this.flagsBitMap == 0) {
      this.gameWorldSnapshotDelta.entitySnapshotDeltas.push(this);
    }
    
    this.flagsBitMap = BooleanUtil.set(this.flagsBitMap, bitMapFlag);
  }

  public computeByteSize(flagsBitMap: number=this.flagsBitMap): number {
    let len: number = 0;

    // Entity id.
    len += 4;

    // The flags bit map.
    len += 4;
    
    if(BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.ownerPlayerId)) {
      len += 4;
    }
    if(BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.pos)) {
      len += 4 * 3;
    }
    if(BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.rot)) {
      len += 4 * 4;
    }
    if(BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.entityRemoved)) {
      len += 0;
    }
    if(BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.entityTemplateId)) {
      len += StreamBuffer.stringInBufferSize(this.template.id);
    }

    for(let i:number=0;i<this.modulesList.length;i++) {
      let entry: Module = this.modulesList[i];
      len += entry.computeByteSize(flagsBitMap);
    }

    return len;
  }

  public writeToBuffer(buff: StreamBuffer, flagsBitMap: number=this.flagsBitMap): void {
    buff.writeInt(this.entityId);
    
    if(flagsBitMap == 0) {
      throw new LogicError('Error.');
    }

    GlobalAccessLogger.getLog().debug("Entity sync. entityId: " + this.entityId + ", flagsBitMap: " + flagsBitMap + ", template.id: " + this.template.id);
    buff.writeInt(flagsBitMap);

    if(BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.ownerPlayerId)) {
      if(this.getOwnerPlayer() != null) {
        buff.writeInt(this.getOwnerPlayer().playerId);
      }else {
        buff.writeInt(0);
      }
    }
    if(BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.pos)) {
      buff.writeFloat(this.getPos().x);
      buff.writeFloat(this.getPos().y);
      buff.writeFloat(this.getPos().z);
    }
    if(BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.rot)) {
      buff.writeFloat(this.getRot().x);
      buff.writeFloat(this.getRot().y);
      buff.writeFloat(this.getRot().z);
      buff.writeFloat(this.getRot().w);
    }
    if(BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.entityRemoved)) {
      //void
    }
    if(BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.entityTemplateId)) {
      buff.writeString(this.template.id);
    }

    for(let i:number=0;i<this.modulesList.length;i++) {
      let entry: Module = this.modulesList[i];
      entry.writeToBuffer(buff, flagsBitMap);
    }
  }

  protected computeFullSyncFlagsBitMap(flagsBitMap: number): number {
    flagsBitMap = BooleanUtil.set(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.ownerPlayerId);
    flagsBitMap = BooleanUtil.set(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.pos);
    flagsBitMap = BooleanUtil.set(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.rot);
    flagsBitMap = BooleanUtil.set(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.entityTemplateId);
    
    for(let i:number=0;i<this.modulesList.length;i++) {
      let entry: Module = this.modulesList[i];
      flagsBitMap = entry.computeFullSyncFlagsBitMap(flagsBitMap);
    }

    return flagsBitMap;
  }
  
  public computeByteSize_fullSync(): number {
    let flagsBitMap: number = this.computeFullSyncFlagsBitMap(0);
    
    return this.computeByteSize(flagsBitMap);
  }

  public writeToBuffer_fullSync(buff: StreamBuffer): void {
    let flagsBitMap: number = this.computeFullSyncFlagsBitMap(0);
    
    this.writeToBuffer(buff, flagsBitMap);
  }

  public onOwnerPlayerChanged(): void {
    super.onOwnerPlayerChanged();

    this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.ownerPlayerId);
  }

  public onPosChanged(): void {
    super.onPosChanged();

    this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.pos);
  }
  
  public onRotDChanged(): void {
    super.onRotDChanged();
    
    this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.rot);
  }

  public onEntityRemove_pre(): void {
    super.onEntityRemove_pre();

    this.isEntityRemoved = true;
    
    this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.entityRemoved);
  }
  
}
