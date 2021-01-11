import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";

export class NetUtil {

  public constructor() {
    //void
  }
  
  public static extractParamValue(paramName: string, url: string): string {
    let startIndex: number;
    let endIndex: number;
    
    startIndex = url.indexOf(paramName + '=');
    if(startIndex < 0) {
      return null;
    }
    startIndex += paramName.length + 1;
    
    endIndex = url.indexOf('&', startIndex);
    if(endIndex < 0) {
      endIndex = url.length;
    }
    
    let s: string = url.substring(startIndex, endIndex);
    
    return s;
  }
  
  public static sendSimpleNetMsg(con: GeneralTcpConnection, netMsgCode: number): void {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(netMsgCode);
      // END : msg data;
      netBlock.endMsgWrite();
      
      con.sendBlock(netBlock);
    } catch (x) {
      throw x;
    }
  }
  
}
