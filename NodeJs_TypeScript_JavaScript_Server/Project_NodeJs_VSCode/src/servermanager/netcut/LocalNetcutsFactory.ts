import { BasicNetcutsFactory } from "base/net/BasicNetcutsFactory";

import { IApp } from "base/app/IApp";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";

import { NetcutFactoryConfig } from "base/net/util/NetcutFactoryConfig";
import { Netcut_Component_To_Component } from "base/common/net/netcut/Netcut_Component_To_Component";

import { Netcut_SC_ServerManager_To_NodeController } from "./Netcut_SC_ServerManager_To_NodeController";
import { Netcut_SC_ServerManager_To_Component } from "./Netcut_SC_ServerManager_To_Component";
import { Netcut_CS_Component_To_LogManager } from "base/logmanager/netcut/Netcut_CS_Component_To_LogManager";
import { IAppLocal } from "servermanager/app/IAppLocal";

export class LocalNetcutsFactory extends BasicNetcutsFactory {

  public constructor() {
    super();

    this.initEntries([
      // As server.
      new NetcutFactoryConfig(
        BasicNetcutsFactory.CONNECTION_TYPE_SERVER,
        ComponentType.NODE_CONTROLLER, 
        function(app: IApp, con: GeneralTcpConnection, componentType: ComponentType): Netcut_Component_To_Component {
          return new Netcut_SC_ServerManager_To_NodeController(<IAppLocal>app, con, componentType)
        }.bind(this)
      ),
      new NetcutFactoryConfig(
        BasicNetcutsFactory.CONNECTION_TYPE_SERVER,
        // Default entry if null.
        null, 
        function(app: IApp, con: GeneralTcpConnection, componentType: ComponentType): Netcut_Component_To_Component {
          return new Netcut_SC_ServerManager_To_Component(<IAppLocal>app, con, componentType)
        }.bind(this)
      ),

      // As client.
      new NetcutFactoryConfig(
        BasicNetcutsFactory.CONNECTION_TYPE_CLIENT,
        ComponentType.LOG_MANAGER, 
        function(app: IApp, con: GeneralTcpConnection, componentType: ComponentType): Netcut_Component_To_Component {
          return new Netcut_CS_Component_To_LogManager(app, con, componentType)
        }.bind(this)
      )
    ]);
  }

}
