import { LimitedCommandNetClient } from "base/common/client/net/holder/util/LimitedCommandNetClient";

import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";

export class ClientAccountsNetClient extends LimitedCommandNetClient {

  public deviceTestString: string = null;

  constructor(con: GeneralTcpConnection) {
    super(con);
    
    //void
  }

}
