import { GWIExtenablePersistentIsolatedInstance } from "localbase/gwi/GWIExtenablePersistentIsolatedInstance";
import { IHandler_GWI } from "localbase/gwi/IHandler_GWI";
import { DTGWIConfigBasicZone } from "./util/DTGWIConfigBasicZone";

export abstract class GWILBase extends GWIExtenablePersistentIsolatedInstance {

  public constructor(handler: IHandler_GWI, config: DTGWIConfigBasicZone) {
    super(handler, config);
    
    //void
  }
  
}
