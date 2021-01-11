import { Netcut_Component_To_Component } from "base/common/net/netcut/Netcut_Component_To_Component";

import { IApp } from "base/app/IApp";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";

export class NetcutFactoryConfig {

    public connectionType: number;

    public componentType: ComponentType;
    
    public newInstance: (app: IApp, con: GeneralTcpConnection, componentType: ComponentType) => Netcut_Component_To_Component;

    public constructor(
        connectionType: number, 
        componentType: ComponentType, 
        newInstance: (app: IApp, con: GeneralTcpConnection, componentType: ComponentType) => Netcut_Component_To_Component) 
    {
        this.connectionType = connectionType;

        this.componentType = componentType;
        
        this.newInstance = newInstance;
    }  

}
