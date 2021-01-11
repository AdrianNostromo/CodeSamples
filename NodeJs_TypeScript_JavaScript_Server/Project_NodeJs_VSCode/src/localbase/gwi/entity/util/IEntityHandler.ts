import { BasicWorldEntity } from "../BasicWorldEntity";
import { EntityRemoveReason } from "./EntityRemoveReason";

export interface IEntityHandler {

  markEntityForRemoval(entity: BasicWorldEntity, removeReason: EntityRemoveReason): void;
  getEntityWithEntityId<T extends BasicWorldEntity>(entityId: number): T;
  
}
