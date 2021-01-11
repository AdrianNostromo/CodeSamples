import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "./../ComponentType";

export class ServerConnectionCallbacksHolder {

  private static uLogIdCounter: number = 0;

  public cbUnvalidatedConnection: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void;
  public cbConnectionFail: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void;
  public cbValidated: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void;

  constructor(
    cbUnvalidatedConnection: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void, 
    cbConnectionFail: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void, 
    cbValidated: (con: GeneralTcpConnection, remoteComponentType: ComponentType) => void
  ) {
    this.cbUnvalidatedConnection = cbUnvalidatedConnection;
    this.cbConnectionFail = cbConnectionFail;
    this.cbValidated = cbValidated;
  }
  
}
