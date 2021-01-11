import { ActionIds } from "./ActionIds";
import { BasicAction } from "localbase/gwi/action/BasicAction";

export class JoinNexusTeamByUnitAction extends BasicAction {
  
  public static readonly TYPE = ActionIds.joinNexusTeamByUnit;
  
  public entityId: number;
  public targetNexusEntityId: number;

  public constructor(
    type: number,

    entityId: number,
    targetNexusEntityId: number
  ) {
    super(type);
    
    this.entityId = entityId;
    this.targetNexusEntityId = targetNexusEntityId;
  }

}
  