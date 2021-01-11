import { ComponentType } from "base/common/ComponentType";
import { AppDirs } from "base/common/util/AppDirs";
import { ServerConnectorNetConfig } from "base/module/util/ServerConnectorNetConfig";

import { NodeController } from "./../NodeController";
import { LocalNetcutsFactory } from "./../netcut/LocalNetcutsFactory";
import { IAppLocal } from "./IAppLocal";
import { App } from "base/app/App";

export class AppLocal extends App implements IAppLocal {

    public nodeController: NodeController = null;

    public constructor(componentType: ComponentType, dirs: AppDirs) {
        super(
            componentType, dirs, 
            new LocalNetcutsFactory(), 
            null, 
            new ServerConnectorNetConfig()
        );
        
        this.initApp(this);
    }

    public getApp(): IAppLocal {
        return this;
    }

    public getNodeController(): NodeController {
        return this.nodeController;
    }

    protected create_framework(): void {
        super.create_framework();
        
        this.addComponent(this.nodeController = new NodeController(this.getApp()));
    }
  
}
