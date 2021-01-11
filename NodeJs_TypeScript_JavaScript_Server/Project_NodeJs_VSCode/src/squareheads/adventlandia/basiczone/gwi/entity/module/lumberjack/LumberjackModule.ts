import { LumberjackModuleSnapshotDelta } from "./LumberjackModuleSnapshotDelta";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { LumberjackTemplate } from "../../template/LumberjackTemplate";

export class LumberjackModule extends LumberjackModuleSnapshotDelta {

  public constructor(entity: WorldEntityLocal, template: LumberjackTemplate) {
    super(entity, template);

    //void
  }
  
}
