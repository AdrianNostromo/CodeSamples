import { BasicWorldEntity } from "localbase/gwi/entity/BasicWorldEntity";
import { MovingEntityModule } from "./module/moving/MovingEntityModule";
import { LivingEntityModule } from "./module/living/LivingEntityModule";
import { LumberjackModule } from "./module/lumberjack/LumberjackModule";
import { VoxelInventoryModule } from "./module/voxelinventory/VoxelInventoryModule";
import { BuilderModule } from "./module/builder/BuilderModule";
import { TeamModule } from "./module/team/TeamModule";
import { EntityTemplate } from "localbase/gwi/entity/template/EntityTemplate";
import { IEntityHandlerLocal } from "./util/IEntityHandlerLocal";
import { GameWorldSnapshotDelta } from "localbase/gwi/sync/GameWorldSnapshotDelta";

export class WorldEntityLocal extends BasicWorldEntity {

  public movingEntity: MovingEntityModule = null;
  public livingEntity: LivingEntityModule = null;
  public lumberjack: LumberjackModule = null;
  public voxelInventory: VoxelInventoryModule = null;
  public builder: BuilderModule = null;
  public team: TeamModule = null;

  public constructor(template: EntityTemplate, entityId: number, handler: IEntityHandlerLocal, gameWorldSnapshotDelta: GameWorldSnapshotDelta) {
    super(template, entityId, handler, gameWorldSnapshotDelta);
    
    //void
  }

  public getHandler(): IEntityHandlerLocal {
    return <IEntityHandlerLocal>this.handler;
  }

}
