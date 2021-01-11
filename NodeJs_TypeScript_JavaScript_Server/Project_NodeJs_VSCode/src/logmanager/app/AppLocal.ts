import { ComponentType } from "base/common/ComponentType";
import { AppDirs } from "base/common/util/AppDirs";
import { ClientListenerNetConfig } from "base/module/util/ClientListenerNetConfig";
import { ServerConnectorNetConfig } from "base/module/util/ServerConnectorNetConfig";
import { IpPort } from "base/net/util/IpPort";

import { LogManager } from "./../LogManager";
import { LocalNetcutsFactory } from "./../netcut/LocalNetcutsFactory";
import { IAppLocal } from "./IAppLocal";
import { App } from "base/app/App";

export class AppLocal extends App implements IAppLocal {

    public logManager: LogManager = null;

    public constructor(componentType: ComponentType, dirs: AppDirs) {
        super(
            componentType, dirs, 
            new LocalNetcutsFactory(), 
            new ClientListenerNetConfig(new IpPort(
                '127.0.0.1',
                0
            )), 
            new ServerConnectorNetConfig()
        );
        
        this.initApp(this);
    }

    public getApp(): IAppLocal {
        return this;
    }

    public getLogManager(): LogManager {
        return this.logManager;
    }
    
    protected create_framework(): void {
        super.create_framework();
        
        this.addComponent(this.logManager = new LogManager(this.getApp()));
    }
    
}
