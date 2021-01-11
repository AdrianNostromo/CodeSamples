import { GWIPlayerActions } from "./GWIPlayerActions";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";
import { BasicPlayer } from "./util/BasicPlayer";
import { NetClient } from "./util/NetClient";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { BasicAction } from "./action/BasicAction";
import { SecurityWarning } from "base/exception/SecurityWarning";

class NetCodes {
  
  public static readonly CS_C_HandlePlayerActions: number = 500010;
  // actionsCount: number;
  // actionsBuffer: Buffer;
  public static readonly SC_R_HandlePlayerActions_Ok: number = 500011;
  
}

export abstract class GWIPlayerActionsNetHandling extends GWIPlayerActions {

  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }

  protected processPlayerMsg(pl: BasicPlayer, client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    if(super.processPlayerMsg(pl, client, msgLen, msgType, msgNetBuffer)) {
      return true;
    }
    
    switch (msgType) {
      case NetCodes.CS_C_HandlePlayerActions:
        this.processMsg_CS_C_HandlePlayerActions(pl, client, msgLen, msgType, msgNetBuffer);
        
        break;
      default:
        return false;
    }
    
    return true;
  }

  private processMsg_CS_C_HandlePlayerActions(pl: BasicPlayer, client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): void {
    let actionsCount: number = msgNetBuffer.readInt();

    for(let i:number=0;i<actionsCount;i++) {
      let action: BasicAction = this.processMsgPart_readPlayerActionEntry(msgNetBuffer);
      if(action == null) {
        throw new SecurityWarning("Logged security warning.");
      }

      // Save the action to a list.
      this.pushPlayerAction(pl, action);
    }
  }

  protected abstract processMsgPart_readPlayerActionEntry(msgNetBuffer: StreamBuffer): BasicAction;

}
