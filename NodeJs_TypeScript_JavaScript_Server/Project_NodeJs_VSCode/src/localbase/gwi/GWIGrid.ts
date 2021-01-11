import { GWIMatchJoiner } from "./GWIMatchJoiner";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";

export abstract class GWIGrid extends GWIMatchJoiner {

  protected static VOXEL_SIZE: number = 0.1;
  
  protected static meterToGrid(posM: number): number {
      return Math.round(posM / GWIGrid.VOXEL_SIZE);
  }
  
  protected static gridToMeter(posG: number): number {
      return posG * GWIGrid.VOXEL_SIZE;
  }
  
  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }

}
