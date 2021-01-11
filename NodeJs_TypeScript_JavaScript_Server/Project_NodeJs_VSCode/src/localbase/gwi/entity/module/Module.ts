import { ModuleSnapshotDelta } from "./ModuleSnapshotDelta";
import { BasicWorldEntity } from "./../BasicWorldEntity";

export class Module extends ModuleSnapshotDelta {

  public readonly usesGameLoop: boolean;

  public constructor(moduleId: string, entity: BasicWorldEntity, usesGameLoop: boolean) {
    super(moduleId, entity, usesGameLoop);
    
    this.entity.selfAddModule(this);
  }

}
