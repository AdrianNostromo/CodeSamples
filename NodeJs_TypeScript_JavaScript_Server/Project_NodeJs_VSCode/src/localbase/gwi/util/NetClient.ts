import { LimitedCommandNetClient } from "base/common/client/net/holder/util/LimitedCommandNetClient";

import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { BasicPlayer } from "./BasicPlayer";

export class NetClient extends LimitedCommandNetClient {

  public accountId: number;

  public player: BasicPlayer = null;

  public lastSyncDeltaIndex: number = 0;
  public activeSyncCommands: number = 0;

  constructor(
    con: GeneralTcpConnection,
    
    accountId: number
  ) {
    super(con);

    this.accountId = accountId
  }
  
}
