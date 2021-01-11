import { ActionIds } from "./ActionIds";
import { BasicAction } from "localbase/gwi/action/BasicAction";

export class TurnAction extends BasicAction {
  
  public static readonly TYPE = ActionIds.turn;

  public entityId: number;
  public rotDZ: number;

  public constructor(
    type: number,

    entityId: number,
    rotDZ: number
  ) {
    super(type);
    
    this.entityId = entityId;
    this.rotDZ = rotDZ;
  }

}
