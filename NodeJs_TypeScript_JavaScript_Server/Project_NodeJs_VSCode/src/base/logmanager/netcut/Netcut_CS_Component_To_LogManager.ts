import { Netcut_CS_ClientComponent_To_ServerComponent } from "base/common/net/netcut/Netcut_CS_ClientComponent_To_ServerComponent";

import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";
import { IApp } from "base/app/IApp";

export class Netcut_CS_Component_To_LogManager extends Netcut_CS_ClientComponent_To_ServerComponent {

  public constructor(app: IApp, con: GeneralTcpConnection, componentType: ComponentType) {
    super(app, con, componentType);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    //void
  }
  
}
