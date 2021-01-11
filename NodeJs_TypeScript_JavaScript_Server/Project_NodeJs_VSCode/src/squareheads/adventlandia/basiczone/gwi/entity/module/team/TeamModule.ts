import { TeamModuleSnapshotDelta } from "./TeamModuleSnapshotDelta";
import { WorldEntityLocal } from "../../WorldEntityLocal";
import { TeamTemplate } from "../../template/TeamTemplate";

export class TeamModule extends TeamModuleSnapshotDelta {

  public constructor(entity: WorldEntityLocal, template: TeamTemplate) {
    super(entity, template);

    //void
  }

}
