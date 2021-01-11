import { GWIGrid } from "./GWIGrid";
import { IEntityHandler } from "./entity/util/IEntityHandler";
import { BasicWorldEntity } from "./entity/BasicWorldEntity";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";
import { Module } from "./entity/module/Module";
import { LogicError } from "base/exception/LogicError";
import { BasicPlayer } from "./util/BasicPlayer";
import { Pos3D } from "base/util/Pos3D";
import { Quaternion3D } from "base/util/Quaternion3D";
import { EntityTemplate } from "./entity/template/EntityTemplate";
import { BasicModuleTemplate } from "./entity/template/BasicModuleTemplate";
import { EntityRemoveReason } from "./entity/util/EntityRemoveReason";

export abstract class GWIEntities extends GWIGrid implements IEntityHandler {

  private entitiesList: BasicWorldEntity[] = [];
  private entitiesMap:Map<string, BasicWorldEntity> = new Map();

  private entitiesList_withGameLoop: BasicWorldEntity[] = [];

  private markedForRemovalEntities: BasicWorldEntity[] = [];

  private entityIdIncrementor: number = 0;
  
  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }

  protected getEntitiesList(): BasicWorldEntity[] {
    return this.entitiesList;
  }

  protected gameLoop_simulation(deltaS: number, deltaMS: number): void {
    super.gameLoop_simulation(deltaS, deltaMS);
    
    for(let i:number=0;i<this.entitiesList_withGameLoop.length;i++) {
      let entity: BasicWorldEntity = this.entitiesList_withGameLoop[i];
      
      for(let j:number=0;j<entity.modulesList_withGameLoop.length;j++) {
        let mod: Module = entity.modulesList_withGameLoop[j];
        mod.gameLoop(deltaS, deltaMS);
      }
    }
  }

  protected gameLoop_entitiesRemoval(deltaS: number, deltaMS: number): void {
    super.gameLoop_entitiesRemoval(deltaS, deltaMS);
    
    if(this.markedForRemovalEntities.length > 0) {
      for(let i:number=this.markedForRemovalEntities.length-1;i>=0;i--) {
        let entity: BasicWorldEntity = this.markedForRemovalEntities[i];

        if(!entity.isMarkedForRemoval) {
          throw new LogicError("GameWorldEntities.createEntity.1.");
        }

        this.removeEntity(entity);
      }

      this.markedForRemovalEntities = [];
    }
  }

  /**
   * 
   * @param template 
   * @param pos 
   * @param rot 
   * @param instanceId 
   * @param isPlayerControllable
   */
  protected createEntity(
    templateId: string, 
    instanceId: string, 
    ownerPlayer: BasicPlayer, 
    
    pos: Pos3D, 
    rot: Quaternion3D
  ): BasicWorldEntity {
    let template: EntityTemplate = EntityTemplate.entriesMap.get(templateId);
    if(template == null) {
      throw new LogicError("GameWorldEntities.createEntity.1.");
    }

    let entityId: number = ++this.entityIdIncrementor;

    let entity: BasicWorldEntity = this.createEntityObject(template, entityId);
    
    if(ownerPlayer != null) {
      entity.setOwnerPlayer(ownerPlayer);
    }

    entity.setPos(pos);
    entity.setRot(rot);
    
    if(instanceId != null) {
      entity.setInstanceId(instanceId);

      // Save the entity in the entities map.
      this.entitiesMap.set(instanceId, entity);
    }
    
    this.entitiesList.push(entity);

    if(entity.modulesList_withGameLoop.length > 0) {
      this.entitiesList_withGameLoop.push(entity);
    }

    this.onEntityCreated(entity);
    
    return entity;
  }

  protected onPlayerRemove_pre(pl: BasicPlayer) {
    super.onPlayerRemove_pre(pl);

    //asdz;// remove all the player entities when he is removed.
  }

  protected createEntityObject(template: EntityTemplate, entityId: number): BasicWorldEntity {
    let entity: BasicWorldEntity = this.createEntity_base(template, entityId);
    if(entity == null) {
      throw new LogicError("GameWorldEntities.createEntity.2.");
    }

    for(let i: number=0;i<entity.template.moduleTemplatesList.length;i++) {
      let subTemplate: BasicModuleTemplate = entity.template.moduleTemplatesList[i];
      
      if(subTemplate.hasModule && !this.initEntityModule(entity, subTemplate)) {
          throw new LogicError("GameWorldEntitiesCreator.createUnit_visual3D.1.");
      }
    }

    return entity;
  }

  protected initEntityModule(entity: BasicWorldEntity, subTemplate: BasicModuleTemplate): boolean {
    //void

    return false;
  }

  // Extendable (mandatory).
  protected abstract createEntity_base(template: EntityTemplate, entityId: number): BasicWorldEntity;

  // Extendable.
  protected onEntityCreated(entity: BasicWorldEntity): void {
    //void
  }

  // Extendable.
  protected onEntityRemoved_pre(entity: BasicWorldEntity): void {
    //void
  }
  
  public markEntityForRemoval(entity: BasicWorldEntity, removeReason: EntityRemoveReason): void {
    entity.isMarkedForRemoval = true;
    this.markedForRemovalEntities.push(entity);
  }
  
  private removeEntity(entity: BasicWorldEntity): void {
    let index: number = this.entitiesList.indexOf(entity);
    if (index < 0) {
      throw new LogicError("GameWorldBase.upgradePreInitMatch.1.");
    }

    entity.onEntityRemove_pre();

    this.onEntityRemoved_pre(entity);

    let j: number = this.entitiesList_withGameLoop.indexOf(entity);
    if(j >= 0) {
      this.entitiesList_withGameLoop.splice(j, 1);
    }

    if(entity.getInstanceId() != null) {
      // Remove the entity from the entities map.
      if(this.entitiesMap.get(entity.getInstanceId()) == null) {
        throw new LogicError("GameWorldEntities.removeEntity.1.");
      }

      this.entitiesMap.delete(entity.getInstanceId());

      entity.setInstanceId(null);
    }

    this.entitiesList.splice(index, 1);
  }

  protected getFirstNotRemovingAnyEntityWhereOwnerPlayer(pl: BasicPlayer): BasicWorldEntity {
    for(let i:number=0;i<this.entitiesList.length;i++) {
      let entity: BasicWorldEntity = this.entitiesList[i];
      if(entity.getOwnerPlayer() == pl && !entity.isMarkedForRemoval) {
        return entity;
      }
    }

    return null;
  }

  public getEntityWithEntityId<T extends BasicWorldEntity>(entityId: number): T {
    for(let i:number=0;i<this.entitiesList.length;i++) {
      let entity: BasicWorldEntity = this.entitiesList[i];
      if(entity != null && entity.entityId == entityId) {
        return <T>entity;
      }
    }

    return null;
  }

  protected removeAllEntitiesWhereOwnerPlayer(pl: BasicPlayer): number {
    let ct: number = 0;
    while(true) {
      let entity: BasicWorldEntity = this.getFirstNotRemovingAnyEntityWhereOwnerPlayer(pl);

      if(entity == null) {
        break;
      }

      this.markEntityForRemoval(entity, EntityRemoveReason.CODE_DISPOSED);
      ct++;
    }

    return ct;
  }
  
  public dispose(): void {
    if(this.entitiesList != null) {
      for(let i: number=this.entitiesList.length-1;i>=0;i--) {
        let entity: BasicWorldEntity = this.entitiesList[i];

        if(entity != null) {
          this.removeEntity(entity);
        }
      }

      this.entitiesList = null;
    }
    
    super.dispose();
  }
  
}
