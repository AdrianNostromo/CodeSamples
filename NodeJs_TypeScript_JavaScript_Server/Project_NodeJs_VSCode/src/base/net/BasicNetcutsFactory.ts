import { NetcutFactoryConfig } from "./util/NetcutFactoryConfig";
import { ComponentType } from "base/common/ComponentType";

export abstract class BasicNetcutsFactory {

  public static readonly CONNECTION_TYPE_SERVER: number = 1;
  public static readonly CONNECTION_TYPE_CLIENT: number = 2;

  private netcutConfigsList: NetcutFactoryConfig[];

  public constructor() {
    //void
  }

  protected initEntries(netcutConfigsList: NetcutFactoryConfig[]) {
    this.netcutConfigsList = netcutConfigsList;
  }

  public getNetcutConfig(connectionType: number, componentType: ComponentType): NetcutFactoryConfig {
    for(let i: number=0;i<this.netcutConfigsList.length;i++) {
      let entry: NetcutFactoryConfig = this.netcutConfigsList[i];
      if(entry != null 
        && entry.connectionType == connectionType 
        && entry.componentType == componentType)
      {
        return entry;
      }
    }
    
    return null;
  }

}
