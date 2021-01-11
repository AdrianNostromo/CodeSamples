import { Pos3D } from "base/util/Pos3D";
import { ActionIds } from "./ActionIds";
import { BasicAction } from "localbase/gwi/action/BasicAction";

export class BuildNexusByUnitAction extends BasicAction {
  
  public static readonly TYPE = ActionIds.createNexusByUnit;
  
  public entityId: number;
  public buildPos: Pos3D;

  public constructor(
    type: number,

    entityId: number,
    buildPos: Pos3D
  ) {
    super(type);
    
    this.entityId = entityId;
    this.buildPos = buildPos;
  }

}
  