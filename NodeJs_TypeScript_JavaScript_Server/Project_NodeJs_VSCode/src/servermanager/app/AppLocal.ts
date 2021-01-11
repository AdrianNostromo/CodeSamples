import { ComponentType } from "base/common/ComponentType";
import { AppDirs } from "base/common/util/AppDirs";
import { ClientListenerNetConfig } from "base/module/util/ClientListenerNetConfig";
import { ServerConnectorNetConfig } from "base/module/util/ServerConnectorNetConfig";
import { AppConfig } from "base/common/config/AppConfig";
import { IpPort } from "base/net/util/IpPort";

import { ServerManager } from "./../ServerManager";
import { LocalNetcutsFactory } from "./../netcut/LocalNetcutsFactory";
import { IAppLocal } from "./IAppLocal";
import { App } from "base/app/App";

export class AppLocal extends App implements IAppLocal {

    public serverManager: ServerManager = null;

    public constructor(componentType: ComponentType, dirs: AppDirs) {
        super(
            componentType, dirs, 
            new LocalNetcutsFactory(), 
            new ClientListenerNetConfig(new IpPort(
                '127.0.0.1',
                AppConfig.DEFAULT_SERVER_MANAGER_IP_PORT.port
            )), 
            new ServerConnectorNetConfig()
        );
        
        this.initApp(this);
    }

    public getApp(): IAppLocal {
        return this;
    }
    
    public getServerManager(): ServerManager {
        return this.serverManager;
    }

    protected create_framework(): void {
        super.create_framework();
        
        this.addComponent(this.serverManager = new ServerManager(this.getApp()));
    }
    
}
