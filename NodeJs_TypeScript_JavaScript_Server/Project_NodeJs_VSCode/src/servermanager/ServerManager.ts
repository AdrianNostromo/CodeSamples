import { LocalCustomModule } from "./module/LocalCustomModule";

import { IAppLocal } from "servermanager/app/IAppLocal";

import { ServerStructureHandler } from "servermanager/serverstructurehandler/ServerStructureHandler";

export class ServerManager extends LocalCustomModule {

  private _serverStructureHandler: ServerStructureHandler = null;

  public constructor(app: IAppLocal) {
    super(
      app, 
      [
        //void
      ]
    );
    
    this._serverStructureHandler = null;
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    this.addComponent(this._serverStructureHandler = new ServerStructureHandler(this.getApp()));
  }

}
