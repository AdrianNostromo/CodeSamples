import { Pos3D } from "base/util/Pos3D";
import { ActionIds } from "./ActionIds";
import { BasicAction } from "localbase/gwi/action/BasicAction";

export class ChopTreeAction extends BasicAction {
  
  public static readonly TYPE = ActionIds.chopTree;
  
  public entityId: number;

  public treeEntityId: number;
  // These are required in case the tree is already destroyed. The chop operation will still occur on a phantom tree.
  public treePos: Pos3D;
  public sortedDynamicVoxelIndex: number;
  public collectedItemId: string;

  public constructor(
    type: number,

    entityId: number,
    treeEntityId: number,
    treePos: Pos3D,
    sortedDynamicVoxelIndex: number,
    collectedItemId: string
  ) {
    super(type);
    
    this.entityId = entityId;
    this.treeEntityId = treeEntityId;
    this.treePos = treePos;
    this.sortedDynamicVoxelIndex = sortedDynamicVoxelIndex;
    this.collectedItemId = collectedItemId;
  }

}
  