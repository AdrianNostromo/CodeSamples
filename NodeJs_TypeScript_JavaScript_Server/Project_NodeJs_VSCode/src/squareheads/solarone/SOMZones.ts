import { SolarOneManagerBase } from "./SolarOneManagerBase";
import { IAppLocal } from "squareheads/app/IAppLocal";
import { ZonesListManager } from "./zoneslistmanager/ZonesListManager";
import { ZonesListInteractor } from "./zoneslistinteractor/ZonesListInteractor";

export class SOMZones extends SolarOneManagerBase {

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
    
}
