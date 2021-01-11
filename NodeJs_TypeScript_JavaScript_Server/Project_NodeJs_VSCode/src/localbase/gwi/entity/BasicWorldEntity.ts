import { BasicWorldEntitySnapshotDelta } from "./BasicWorldEntitySnapshotDelta";
import { EntityTemplate } from "./template/EntityTemplate";
import { GameWorldSnapshotDelta } from "./../sync/GameWorldSnapshotDelta";
import { IEntityHandler } from "./util/IEntityHandler";

export class BasicWorldEntity extends BasicWorldEntitySnapshotDelta {

  public constructor(template: EntityTemplate, entityId: number, handler: IEntityHandler, gameWorldSnapshotDelta: GameWorldSnapshotDelta) {
    super(template, entityId, handler, gameWorldSnapshotDelta);

    //void
  }

}
