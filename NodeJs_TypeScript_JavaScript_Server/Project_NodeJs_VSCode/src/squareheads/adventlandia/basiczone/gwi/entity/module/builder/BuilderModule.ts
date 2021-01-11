import { BuilderModuleSnapshotDelta } from "./BuilderModuleSnapshotDelta";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { BuilderTemplate } from "../../template/BuilderTemplate";

export class BuilderModule extends BuilderModuleSnapshotDelta {

  public constructor(entity: WorldEntityLocal, template: BuilderTemplate) {
    super(entity, template);

    //void
  }

}
