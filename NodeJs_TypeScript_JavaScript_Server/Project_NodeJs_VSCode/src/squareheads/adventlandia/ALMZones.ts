import { IAppLocal } from "squareheads/app/IAppLocal";
import { ZonesListManager } from "./zoneslistmanager/ZonesListManager";
import { ZonesListInteractor } from "./zoneslistinteractor/ZonesListInteractor";
import { ALMPlayerContent } from "./ALMPlayerContent";

export class ALMZones extends ALMPlayerContent {

    public zonesListManager: ZonesListManager = null;
    public zonesListInteractor: ZonesListInteractor = null;
  
    public constructor(app: IAppLocal) {
      super(app);
      
      //void
    }
    
    protected create_framework(): void {
      super.create_framework();
      
      this.addComponent(this.zonesListManager = new ZonesListManager(this.getApp()));
      
      this.addComponent(this.zonesListInteractor = new ZonesListInteractor(this.getApp()));
    }
    
    public getZonesListManager(): ZonesListManager {
        return this.zonesListManager;
    }

    public getZonesListInteractor(): ZonesListInteractor {
        return this.zonesListInteractor;
    }

}
