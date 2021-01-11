import { IBasicGWI } from "./IBasicGWI";
import { ClientDevicesManager } from "squareheads/clientdevicesmanager/ClientDevicesManager";
import { ClientSessionsManager } from "squareheads/clientsessionsmanager/ClientSessionsManager";
import { ILogger } from "base/log/ILogger";

export interface IHandler_GWI {

  onNetError(target: IBasicGWI): void;
  onLogicError(target: IBasicGWI): void;

  getClientDevicesManager(): ClientDevicesManager;
  getClientSessionsManager(): ClientSessionsManager;

  getLog(): ILogger;

}
