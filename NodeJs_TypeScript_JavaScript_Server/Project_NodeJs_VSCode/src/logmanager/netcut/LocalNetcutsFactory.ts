import { BasicNetcutsFactory } from "base/net/BasicNetcutsFactory";

import { IApp } from "base/app/IApp";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";

import { Netcut_Component_To_Component } from "base/common/net/netcut/Netcut_Component_To_Component";
import { NetcutFactoryConfig } from "base/net/util/NetcutFactoryConfig";

import { Netcut_SC_LogManager_To_Component } from "./Netcut_SC_LogManager_To_Component";

import { IAppLocal } from "logmanager/app/IAppLocal";
import { Netcut_CS_Component_To_ServerManager } from "base/servermanager/netcut/Netcut_CS_Component_To_ServerManager";
import { Netcut_CS_Component_To_LogManager } from "base/logmanager/netcut/Netcut_CS_Component_To_LogManager";

export class LocalNetcutsFactory extends BasicNetcutsFactory {

  constructor() {
    super();

    this.initEntries([
      // As server.
      new NetcutFactoryConfig(
        BasicNetcutsFactory.CONNECTION_TYPE_SERVER,
        null, 
        function(app: IApp, con: GeneralTcpConnection, componentType: ComponentType): Netcut_Component_To_Component {
          return new Netcut_SC_LogManager_To_Component(<IAppLocal>app, con, componentType)
        }.bind(this)
      ),

      // As client.
      new NetcutFactoryConfig(
        BasicNetcutsFactory.CONNECTION_TYPE_CLIENT,
        ComponentType.SERVER_MANAGER, 
        function(app: IApp, con: GeneralTcpConnection, componentType: ComponentType): Netcut_Component_To_Component {
          return new Netcut_CS_Component_To_ServerManager(<IAppLocal>app, con, componentType)
        }.bind(this)
      ),
      // As client.
      new NetcutFactoryConfig(
        BasicNetcutsFactory.CONNECTION_TYPE_CLIENT,
        ComponentType.LOG_MANAGER, 
        function(app: IApp, con: GeneralTcpConnection, componentType: ComponentType): Netcut_Component_To_Component {
          return new Netcut_CS_Component_To_LogManager(<IAppLocal>app, con, componentType)
        }.bind(this)
      )
    ]);
  }
  
}
