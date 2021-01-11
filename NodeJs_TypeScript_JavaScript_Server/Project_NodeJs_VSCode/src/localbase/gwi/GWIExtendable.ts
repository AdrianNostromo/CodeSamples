import { GWIPlayerActionsNetHandling } from "./GWIPlayerActionsNetHandling";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";

export abstract class GWIExtendable extends GWIPlayerActionsNetHandling {

  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }

}
