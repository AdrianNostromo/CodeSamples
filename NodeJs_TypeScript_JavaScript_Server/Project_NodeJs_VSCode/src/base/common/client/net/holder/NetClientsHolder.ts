import { NetClientsHolderList } from "./NetClientsHolderList";

import { BasicNetClient } from "./util/BasicNetClient";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export class NetClientsHolder<T extends BasicNetClient> extends NetClientsHolderList<T> {

  public constructor(
    cbNewClient: (client: BasicNetClient) => void,
    cbClientRemoved_pre: (client: BasicNetClient) => void,
    cbProcessMsg: (client: BasicNetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer) => boolean
  ) {
    super(cbNewClient, cbClientRemoved_pre, cbProcessMsg);
    
    //void
  }
  
}
