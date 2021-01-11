import { AdventLandiaManager } from "./adventlandia/AdventLandiaManager";
import { SolarOneManager } from "./solarone/SolarOneManager";
import { ClientAccountsManager } from "./clientaccountsmanager/ClientAccountsManager";
import { ClientDevicesManager } from "./clientdevicesmanager/ClientDevicesManager";
import { ClientSessionsManager } from "./clientsessionsmanager/ClientSessionsManager";
import { ClientsDatastore } from "./clientsdatastore/ClientsDatastore";
import { ClientAccountsInteractor } from "./clientaccountsinteractor/ClientAccountsInteractor";

export interface ISquareHeadsManager {

    getClientsDatastore(): ClientsDatastore;

    getClientAccountsManager(): ClientAccountsManager;
    getClientDevicesManager(): ClientDevicesManager;
    getClientSessionsManager(): ClientSessionsManager;
    
    getClientAccountsInteractor(): ClientAccountsInteractor;

    getAdventLandia(): AdventLandiaManager;
    getSolarOne(): SolarOneManager;
    
}
