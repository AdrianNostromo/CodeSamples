import { SquareHeadsManagerBase } from "./SquareHeadsManagerBase";

import { IAppLocal } from "./app/IAppLocal";

import { ClientsDatastore } from "./clientsdatastore/ClientsDatastore";
import { ClientAccountsManager } from "./clientaccountsmanager/ClientAccountsManager";
import { ClientDevicesManager } from "./clientdevicesmanager/ClientDevicesManager";
import { ClientSessionsManager } from "./clientsessionsmanager/ClientSessionsManager";
import { ClientAccountsInteractor } from "./clientaccountsinteractor/ClientAccountsInteractor";

export class SquareHeadsManagerUsers extends SquareHeadsManagerBase {

  public clientsDatastore: ClientsDatastore = null;
  
  public clientAccountsManager: ClientAccountsManager = null;
  public clientDevicesManager: ClientDevicesManager = null;
  public clientSessionsManager: ClientSessionsManager = null;
  
  public clientAccountsInteractor: ClientAccountsInteractor = null;
  
  public constructor(app: IAppLocal) {
    super(app);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    this.addComponent(this.clientsDatastore = new ClientsDatastore(this.getApp()));
    
    this.addComponent(this.clientAccountsManager = new ClientAccountsManager(this.getApp()));

    this.addComponent(this.clientDevicesManager = new ClientDevicesManager(this.getApp()));

    this.addComponent(this.clientSessionsManager = new ClientSessionsManager(this.getApp()));

    this.addComponent(this.clientAccountsInteractor = new ClientAccountsInteractor(this.getApp()));
  }

  public getClientsDatastore(): ClientsDatastore {
      return this.clientsDatastore;
  }

  public getClientAccountsManager(): ClientAccountsManager {
      return this.clientAccountsManager;
  }

  public getClientDevicesManager(): ClientDevicesManager {
    return this.clientDevicesManager;
  }

  public getClientSessionsManager(): ClientSessionsManager {
    return this.clientSessionsManager;
  }

  public getClientAccountsInteractor(): ClientAccountsInteractor {
      return this.clientAccountsInteractor;
  }
  
}
