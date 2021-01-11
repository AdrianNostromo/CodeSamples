import { BasicNetClient } from "./BasicNetClient";

import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { LogicError } from "base/exception/LogicError";

export class LimitedCommandNetClient extends BasicNetClient {

  // -1: no command.
  // NOTE : the currentCommandTimeoutS must be set to -1 even if there is no active command.
  private currentCommandNetCode: number = -1;

  private static DEFAULT_COMMAND_TIMEOUT_S: number = 5;
  
  constructor(con: GeneralTcpConnection) {
    super(con);

    //void
  }

  public getCurrentCommandNetCode(): number {
    return this.currentCommandNetCode;
  }

  public setCurrentCommandNetCode(currentCommandNetCode: number): void {
    if(this.currentCommandNetCode != -1) {
      throw new LogicError('Error.');
    }
    this.currentCommandNetCode = currentCommandNetCode;
    this.currentCommandTimeoutS = LimitedCommandNetClient.DEFAULT_COMMAND_TIMEOUT_S;
  }

  public clearCurrentCommandNetCode(): void {
    this.currentCommandNetCode = -1;
    this.currentCommandTimeoutS = -1;
  }

}
