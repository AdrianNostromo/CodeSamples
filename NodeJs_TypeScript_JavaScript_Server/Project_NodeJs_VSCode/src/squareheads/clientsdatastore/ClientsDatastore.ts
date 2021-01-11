import { ClientsDatastoreSessions } from "./ClientsDatastoreSessions";

import { IAppLocal } from "./../app/IAppLocal";

export class ClientsDatastore extends ClientsDatastoreSessions {

  public constructor(app: IAppLocal) {
    super(app);
    
    //void
  }

  protected create_framework(): void {
    // NOTE : database creation code.
    //this.factoryInitializeNewDatabase();

    super.create_framework();
  }

  protected getDbPath(): string {
    //super.getDbPath();
    
    let dbFilePath: string = this.getApp().getDirs().dataDir + 'users/db_clients.json';
    
    return dbFilePath;
  }

}
