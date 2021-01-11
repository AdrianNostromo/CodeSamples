import { Netcut_SC_ServerComponent_To_ClientComponent } from "base/common/net/netcut/Netcut_SC_ServerComponent_To_ClientComponent";

import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";
import { IAppLocal } from "logmanager/app/IAppLocal";

export class Netcut_SC_LogManager_To_Component extends Netcut_SC_ServerComponent_To_ClientComponent {

  public constructor(app: IAppLocal, con: GeneralTcpConnection, componentType: ComponentType) {
    super(app, con, componentType);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    //void
  }
  
}
