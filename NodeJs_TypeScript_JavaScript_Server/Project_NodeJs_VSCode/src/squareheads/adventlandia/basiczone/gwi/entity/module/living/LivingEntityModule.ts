import { LivingEntityModuleSnapshotDelta } from "./LivingEntityModuleSnapshotDelta";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { LivingEntityTemplate } from "../../template/LivingEntityTemplate";

export class LivingEntityModule extends LivingEntityModuleSnapshotDelta {

  public constructor(entity: WorldEntityLocal, template: LivingEntityTemplate) {
    super(entity, template);

    //void
  }
  
}
