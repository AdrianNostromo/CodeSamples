import { GWILEntities } from "./GWILEntities";
import { IHandler_GWI } from "localbase/gwi/IHandler_GWI";
import { BasicPlayer } from "localbase/gwi/util/BasicPlayer";
import { BasicWorldEntity } from "localbase/gwi/entity/BasicWorldEntity";
import { EntityTemplatesLocal } from "./config/EntityTemplatesLocal";
import { Pos3D } from "base/util/Pos3D";
import { Quaternion3D } from "base/util/Quaternion3D";
import { Vector3 } from "base/util/Vector3";
import { BasicModuleTemplate } from "localbase/gwi/entity/template/BasicModuleTemplate";
import { MovingEntityTemplate } from "./entity/template/MovingEntityTemplate";
import { WorldEntityLocal } from "./entity/WorldEntityLocal";
import { LivingEntityTemplate } from "./entity/template/LivingEntityTemplate";
import { VoxelInventoryTemplate } from "./entity/template/VoxelInventoryTemplate";
import { LumberjackTemplate } from "./entity/template/LumberjackTemplate";
import { BuilderTemplate } from "./entity/template/BuilderTemplate";
import { TeamTemplate } from "./entity/template/TeamTemplate";
import { MovingEntityModule } from "./entity/module/moving/MovingEntityModule";
import { LivingEntityModule } from "./entity/module/living/LivingEntityModule";
import { VoxelInventoryModule } from "./entity/module/voxelinventory/VoxelInventoryModule";
import { LumberjackModule } from "./entity/module/lumberjack/LumberjackModule";
import { BuilderModule } from "./entity/module/builder/BuilderModule";
import { TeamModule } from "./entity/module/team/TeamModule";
import { EntityTemplate } from "localbase/gwi/entity/template/EntityTemplate";
import { DTGWIConfigBasicZone } from "./util/DTGWIConfigBasicZone";

export abstract class GWILContentInit extends GWILEntities {

  public constructor(handler: IHandler_GWI, config: DTGWIConfigBasicZone) {
    super(handler, config);
    
    //void
  }
  
  protected create_content(): void {
    super.create_content();

    //void
  }
  
  protected onPlayerCreated(pl: BasicPlayer) {
    super.onPlayerCreated(pl);

    let islandGridDiameter: number = 18 * 10 + 1;
    let islandGridCenterX: number = (islandGridDiameter - 1) / 2;
    let islandGridCenterY: number = (islandGridDiameter - 1) / 2;
    let voxelSize: number = 0.1;
    
    let entity: BasicWorldEntity = this.createEntity(
      EntityTemplatesLocal.Unit_Player.id, 
      null,
      pl,
      new Pos3D(
        islandGridCenterX * voxelSize, 
        islandGridCenterY * voxelSize, 
        0
      ),
      // Quaternion for a Z rotation of -90 degrees (unit facing south towards the camera).
      Quaternion3D.fromAxisAngle(Vector3.Z, -90)
    );
  }

  protected onPlayerRemove_pre(pl: BasicPlayer) {
    super.onPlayerRemove_pre(pl);

    this.removeAllEntitiesWhereOwnerPlayer(pl);
  }

  protected initEntityModule(entity: BasicWorldEntity, subTemplate: BasicModuleTemplate): boolean {
    if(super.initEntityModule(entity, subTemplate)) {
      return true;
    }

    if(subTemplate.moduleId == MovingEntityTemplate.MODULE_ID) {
      this.createUnit_movingEntity(<WorldEntityLocal>entity, <MovingEntityTemplate>subTemplate);
    }else if(subTemplate.moduleId == LivingEntityTemplate.MODULE_ID) {
      this.createUnit_livingEntity(<WorldEntityLocal>entity, <LivingEntityTemplate>subTemplate);
    }else if(subTemplate.moduleId == VoxelInventoryTemplate.MODULE_ID) {
      this.createUnit_voxelInventory(<WorldEntityLocal>entity, <VoxelInventoryTemplate>subTemplate);
    }else if(subTemplate.moduleId == LumberjackTemplate.MODULE_ID) {
      this.createUnit_lumberjack(<WorldEntityLocal>entity, <LumberjackTemplate>subTemplate);
    }else if(subTemplate.moduleId == BuilderTemplate.MODULE_ID) {
      this.createUnit_builder(<WorldEntityLocal>entity, <BuilderTemplate>subTemplate);
    }else if(subTemplate.moduleId == TeamTemplate.MODULE_ID) {
      this.createUnit_team(<WorldEntityLocal>entity, <TeamTemplate>subTemplate);
    }else {
      return false;
    }

    return true;
  }

  private createUnit_movingEntity(entity: WorldEntityLocal, movingEntityTemplate: MovingEntityTemplate): void {
    let movingEntity: MovingEntityModule = new MovingEntityModule(entity, movingEntityTemplate);
    
    entity.movingEntity = movingEntity;
  }

  private createUnit_livingEntity(entity: WorldEntityLocal, livingEntityTemplate: LivingEntityTemplate): void {
    let livingEntity: LivingEntityModule = new LivingEntityModule(entity, livingEntityTemplate);
    
    entity.livingEntity = livingEntity;
  }

  private createUnit_voxelInventory(entity: WorldEntityLocal, visualInventoryTemplate: VoxelInventoryTemplate): void {
    let voxelInventory: VoxelInventoryModule = new VoxelInventoryModule(entity, visualInventoryTemplate);
    
    entity.voxelInventory = voxelInventory;
  }

  private createUnit_lumberjack(entity: WorldEntityLocal, lumberjackTemplate: LumberjackTemplate): void {
    let lumberjack: LumberjackModule = new LumberjackModule(entity, lumberjackTemplate);
    
    entity.lumberjack = lumberjack;
  }

  private createUnit_builder(entity: WorldEntityLocal, builderTemplate: BuilderTemplate): void {
    let builder: BuilderModule = new BuilderModule(entity, builderTemplate);
    
    entity.builder = builder;
  }

  private createUnit_team(entity: WorldEntityLocal, teamTemplate: TeamTemplate): void {
    let team: TeamModule = new TeamModule(entity, teamTemplate);
    
    entity.team = team;
  }

  protected createEntity_base(template: EntityTemplate, entityId: number): BasicWorldEntity {
    let entity: WorldEntityLocal = new WorldEntityLocal(
      template,
      entityId,
      this,
      this.getGameWorldSnapshotDelta()
    );

    return entity;
  }
  
}
