import { GWILBase } from "./GWILBase";
import { IEntityHandlerLocal } from "./entity/util/IEntityHandlerLocal";
import { IHandler_GWI } from "localbase/gwi/IHandler_GWI";
import { DTGWIConfigBasicZone } from "./util/DTGWIConfigBasicZone";

export abstract class GWILEntities extends GWILBase implements IEntityHandlerLocal {

  private nexusTeamIndexCounter: number = 0;

  public constructor(handler: IHandler_GWI, config: DTGWIConfigBasicZone) {
    super(handler, config);
    
    //void
  }

  public reserveNextNexusTeamIndex(): number {
    this.nexusTeamIndexCounter++;

    return this.nexusTeamIndexCounter;
  }
  
}
