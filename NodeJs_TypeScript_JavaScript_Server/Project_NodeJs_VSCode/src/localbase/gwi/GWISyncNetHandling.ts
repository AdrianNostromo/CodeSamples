import { GWIEntities } from "./GWIEntities";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";
import { BasicPlayer } from "./util/BasicPlayer";
import { NetClient } from "./util/NetClient";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { GlobalAccessLogger } from "base/log/GlobalAccessLogger";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";

class NetCodes {
  
  public static readonly SC_C_SINK_GAME_WORLD_STATE: number = 400010;
  // snapshotDeltaIndex: number;
  // updateBufferLength: number;
  // updateBuffer: Buffer;
  public static readonly CS_R_SINK_GAME_WORLD_STATE_OK: number = 400011;
  
}

export abstract class GWISyncNetHandling extends GWIEntities {

  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }

  protected processPlayerMsg(pl: BasicPlayer, client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    if(super.processPlayerMsg(pl, client, msgLen, msgType, msgNetBuffer)) {
      return true;
    }
    
    switch (msgType) {
      case NetCodes.CS_R_SINK_GAME_WORLD_STATE_OK:
        this.processMsg_CS_R_SINK_GAME_WORLD_STATE_OK(pl, client, msgLen, msgType, msgNetBuffer);
        
        break;
      default:
        return false;
    }
    
    return true;
  }

  private processMsg_CS_R_SINK_GAME_WORLD_STATE_OK(pl: BasicPlayer, client: NetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): void {
    if(client.activeSyncCommands <= 0) {
      throw new Error('Security warning.');
    }
    client.activeSyncCommands--;

    //void
  }

  protected sendMsg_SC_C_SINK_GAME_WORLD_STATE(client: NetClient, snapshotDeltaIndex: number, updateBuffer: Buffer): void {
    GlobalAccessLogger.getLog().debug("Client sync msg. snapshotDeltaIndex: " + snapshotDeltaIndex + ", updateBuffer.length: " + updateBuffer.length + ", client.remotePort" + client.remotePort);

    client.activeSyncCommands++;
    client.getCon().sendBlock(new NetMsgsBlock(1, 
      4
      + 4
      + 4
      + updateBuffer.length)
      .startMsgWrite()
      .writeInt(NetCodes.SC_C_SINK_GAME_WORLD_STATE)
      .writeInt(snapshotDeltaIndex)
      .writeInt(updateBuffer.length)
      .writeBuffer(updateBuffer)
      .endMsgWrite()
    );
  }

}
