import { ClientDevicesManagerBase } from "./ClientDevicesManagerBase";

import { IAppLocal } from "./../app/IAppLocal";

import { ConnectedClientDeviceInfo } from "./util/ConnectedClientDeviceInfo";
import { DbConnectedDeviceEntry } from "squareheads/clientsdatastore/util/DbConnectedDeviceEntry";

export class ClientDevicesManager extends ClientDevicesManagerBase {

  public constructor(app: IAppLocal) {
    super(app);
    
    //void
  }

  public connectNewDeviceToAccount(accountId: number): ConnectedClientDeviceInfo {
    let connectedDeviceEntry: DbConnectedDeviceEntry = this.getApp().getSquareHeads().getClientsDatastore().insertConnectedDevice(accountId);
    
    let ret: ConnectedClientDeviceInfo = new ConnectedClientDeviceInfo(
      connectedDeviceEntry.deivceId,
      connectedDeviceEntry.deivceKey,

      connectedDeviceEntry.accountId
    );

    return ret;
  }
  
  public getConnectedDeviceInfoFromDeviceId(connectedDeviceId: number): ConnectedClientDeviceInfo {
    let connectedDeviceEntry: DbConnectedDeviceEntry = this.getApp().getSquareHeads().getClientsDatastore().selectConnectedDeviceWithDeviceId(connectedDeviceId);
    if(connectedDeviceEntry == null) {
      return null;
    }
    
    let ret: ConnectedClientDeviceInfo = new ConnectedClientDeviceInfo(
      connectedDeviceEntry.deivceId,
      connectedDeviceEntry.deivceKey,
      connectedDeviceEntry.accountId
    );

    return ret;
  }
  
}
