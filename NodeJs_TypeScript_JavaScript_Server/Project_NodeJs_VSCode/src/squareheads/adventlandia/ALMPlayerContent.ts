import { AdventLandiaManagerBase } from "./AdventLandiaManagerBase";
import { PlayerContentDatastore } from "./playercontentdatastore/PlayerContentDatastore";
import { PlayerContentManager } from "./playercontentmanager/PlayerContentManager";
import { PlayerContentInteractor } from "./playercontentinteractor/PlayerContentInteractor";
import { IAppLocal } from "squareheads/app/IAppLocal";

export class ALMPlayerContent extends AdventLandiaManagerBase {

    public playerContentDatastore: PlayerContentDatastore = null;
    public playerContentManager: PlayerContentManager = null;
    public playerContentInteractor: PlayerContentInteractor = null;
  
    public constructor(app: IAppLocal) {
        super(app);

        //void
    }

    protected create_framework(): void {
        super.create_framework();
        
        this.addComponent(this.playerContentDatastore = new PlayerContentDatastore(this.getApp()));

        this.addComponent(this.playerContentManager = new PlayerContentManager(this.getApp()));
        
        this.addComponent(this.playerContentInteractor = new PlayerContentInteractor(this.getApp()));
      }
      
      public getPlayerContentDatastore(): PlayerContentDatastore {
          return this.playerContentDatastore;
      }
      
      public getPlayerContentManager(): PlayerContentManager {
          return this.playerContentManager;
      }
  
      public getPlayerContentInteractor(): PlayerContentInteractor {
          return this.playerContentInteractor;
      }
  
}
