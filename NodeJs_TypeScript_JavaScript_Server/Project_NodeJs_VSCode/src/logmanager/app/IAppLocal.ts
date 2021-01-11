import { IApp } from "base/app/IApp";
import { LogManager } from "logmanager/LogManager";

export interface IAppLocal extends IApp {

    getLogManager(): LogManager;
    
}
