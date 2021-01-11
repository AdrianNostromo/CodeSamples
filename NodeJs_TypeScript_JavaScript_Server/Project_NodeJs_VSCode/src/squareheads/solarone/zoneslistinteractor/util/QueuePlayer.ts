import { LimitedCommandNetClient } from "base/common/client/net/holder/util/LimitedCommandNetClient";

import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";

export class QueuePlayer extends LimitedCommandNetClient {

  public accountId: number;

  public isMatchFindInProgress: boolean = false;
  //public isMatchFound: boolean = false;

  constructor(con: GeneralTcpConnection, accountId: number) {
    super(con);
    
    this.accountId = accountId;
  }
  
}
