import { IApp } from "base/app/IApp";
import { ServerManager } from "servermanager/ServerManager";

export interface IAppLocal extends IApp {

    getServerManager(): ServerManager;
    
}
