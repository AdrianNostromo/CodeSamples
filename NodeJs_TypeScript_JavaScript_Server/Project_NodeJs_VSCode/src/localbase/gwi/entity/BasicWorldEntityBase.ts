import { EntityTemplate } from "./template/EntityTemplate";

import { Pos3D } from "base/util/Pos3D";
import { Quaternion3D } from "base/util/Quaternion3D";
import { Module } from "./module/Module";
import { Vector3 } from "base/util/Vector3";
import { IEntityHandler } from "./util/IEntityHandler";
import { BasicPlayer } from "../util/BasicPlayer";

export class BasicWorldEntityBase {

  public template: EntityTemplate;

  public entityId: number;

  public handler: IEntityHandler;

  private instanceId: string;

  protected modulesList: Module[] = [];
  public modulesList_withGameLoop: Module[] = [];
  
  protected modulesMap: Map<string, Module> = new Map();

  private pos: Pos3D = new Pos3D(0, 0, 0);
  private rot: Quaternion3D = new Quaternion3D();

  private ownerPlayer: BasicPlayer = null;

  // This is edited only by GWIEntities.
  public isMarkedForRemoval: boolean = false;

  public constructor(
    template: EntityTemplate,

    entityId: number,

    handler: IEntityHandler
  ) {
    this.template = template;

    this.entityId = entityId;

    this.handler = handler;
  }

  public getInstanceId(): string {
    return this.instanceId;
  }

  public setInstanceId(instanceId: string): void {
    this.instanceId = instanceId;
  }

  public getPos(): Pos3D {
    return this.pos;
  }

  public getX(): number {
    return this.pos.x;
  }

  public getY(): number {
    return this.pos.y;
  }

  public getZ(): number {
    return this.pos.z;
  }

  public setPos(pos: Pos3D) {
    this.setPosXYZ(pos.x, pos.y, pos.z);
  }
  
  public setPosXYZ(x: number, y: number, z: number, skipSnapshotCheck: boolean=false): void {
    if(this.pos.equalsXYZ(x, y, z)) {
      return ;
    }

    this.pos.setXYZ(x, y, z);
    
    if(!skipSnapshotCheck) {
      this.onPosChanged();
    }
  }
  
  protected onPosChanged(): void {
    //void
  }

  public getRot(): Quaternion3D {
    return this.rot;
  }

  public setRot(rot: Quaternion3D, skipSnapshotCheck: boolean=false): void {
    if(this.rot.equalsQ(rot)) {
      return ;
    }
    
    this.rot.setQ(rot);

    if(!skipSnapshotCheck) {
      this.onRotDChanged();
    }
  }

  public setRotAxisAngle(axis: Vector3, degrees: number, skipSnapshotCheck: boolean=false): void {
    this.rot.setAxisAngle(axis, degrees);

    if(!skipSnapshotCheck) {
      this.onRotDChanged();
    }
  }

  protected onRotDChanged(): void {
    //void
  }

  public getOwnerPlayer(): BasicPlayer {
    return this.ownerPlayer;
  }
  
  public setOwnerPlayer(ownerPlayer: BasicPlayer): void {
    if(this.ownerPlayer == ownerPlayer) {
      return ;
    }

    this.ownerPlayer = ownerPlayer;

    this.onOwnerPlayerChanged();
  }

  protected onOwnerPlayerChanged(): void {
    //void
  }

  public onEntityRemove_pre(): void {
    //void
  }

  // The modules register them selfs in their constructor (Module).
  public selfAddModule(mod: Module): void {
    this.modulesList.push(mod);

    if(mod.usesGameLoop) {
      this.modulesList_withGameLoop.push(mod);
    }

    this.modulesMap.set(mod.moduleId, mod);
  }

  public getModule<T extends Module>(moduleId: string): T {
    let mod: Module = this.modulesMap.get(moduleId);
    
    return <T>mod;
  }

}
