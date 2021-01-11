import { LimitedCommandNetClient } from "base/common/client/net/holder/util/LimitedCommandNetClient";

import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";

export class UnvalidatedClient extends LimitedCommandNetClient {

  public posValidationAccountId: number = -1;

  public sessionTestString: string = null;

  constructor(con: GeneralTcpConnection) {
    super(con);

    //void
  }
  
}
