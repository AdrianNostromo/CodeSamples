import { ComponentType } from "base/common/ComponentType";
import { AppDirs } from "base/common/util/AppDirs";
import { ServerConnectorNetConfig } from "base/module/util/ServerConnectorNetConfig";

import { SquareHeadsManager } from "./../SquareHeadsManager";
import { LocalNetcutsFactory } from "./../netcut/LocalNetcutsFactory";
import { IAppLocal } from "./IAppLocal";
import { App } from "base/app/App";
import { ISquareHeadsManager } from "squareheads/ISquareHeadsManager";

export class AppLocal extends App implements IAppLocal {

    private squareHeads: SquareHeadsManager = null;

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

    public getSquareHeads(): ISquareHeadsManager {
        return this.squareHeads;
    }

    protected create_framework(): void {
        super.create_framework();
        
        this.addComponent(this.squareHeads = new SquareHeadsManager(this.getApp()));
    }
  
}
