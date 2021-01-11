import { ActionIds } from "./ActionIds";
import { BasicAction } from "localbase/gwi/action/BasicAction";

export class StopMovementAction extends BasicAction {
  
  public static readonly TYPE = ActionIds.stopMovement;

  public entityId: number;
  
  public constructor(
    type: number,

    entityId: number
  ) {
    super(type);
    
    this.entityId = entityId;
  }

}
  