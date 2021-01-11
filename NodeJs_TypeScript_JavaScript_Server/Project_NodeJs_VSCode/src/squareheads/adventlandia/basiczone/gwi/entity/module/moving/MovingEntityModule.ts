import { MovingEntityModuleSnapshotDelta } from "./MovingEntityModuleSnapshotDelta";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { MovingEntityTemplate } from "../../template/MovingEntityTemplate";

export class MovingEntityModule extends MovingEntityModuleSnapshotDelta {

  public constructor(entity: WorldEntityLocal, template: MovingEntityTemplate) {
    super(entity, template);

    //void
  }
  
}
