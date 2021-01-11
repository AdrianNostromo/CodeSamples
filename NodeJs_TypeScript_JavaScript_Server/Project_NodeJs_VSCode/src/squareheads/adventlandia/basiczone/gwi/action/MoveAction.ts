import { ActionIds } from "./ActionIds";
import { BasicAction } from "localbase/gwi/action/BasicAction";

export class MoveAction extends BasicAction {
  
  public static readonly TYPE = ActionIds.move;

  public entityId: number;
  public destX: number;
  public destY: number;

  public constructor(
    type: number,
    
    entityId: number,
    destX: number,
    destY: number
  ) {
    super(type);
    
    this.entityId = entityId;
    this.destX = destX;
    this.destY = destY;
  }

}
  