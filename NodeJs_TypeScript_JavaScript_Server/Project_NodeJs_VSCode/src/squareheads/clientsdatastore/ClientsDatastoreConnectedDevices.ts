import { ClientsDatastoreUsers } from "./ClientsDatastoreUsers";

import { IAppLocal } from "./../app/IAppLocal";

import { DbConnectedDeviceEntry } from "./util/DbConnectedDeviceEntry";
import { DbAccountEntry } from "./util/DbAccountEntry";
import { LogicError } from "base/exception/LogicError";
import { CryptoUtil } from "base/crypto/util/CryptoUtil";

export abstract class ClientsDatastoreConnectedDevices extends ClientsDatastoreUsers {

  private static DEVICE_KEY_STRING_LENGTH: number = 32;

  private connectedDevicesCollection: Collection<DbConnectedDeviceEntry> = null;

  public constructor(app: IAppLocal) {
    super(app);
    
    //void
  }

  protected factoryInitializeNewCollections(db: Loki): void {
    super.factoryInitializeNewCollections(db);

    db.addCollection<DbConnectedDeviceEntry>('connected_devices', {
      indices: ['deivceId'],
      unique: ['deivceId']
    });
  }

  protected onInitDbColleactions(): void {
    super.onInitDbColleactions();
    
    this.connectedDevicesCollection = this.dbInstance.getCollection<DbConnectedDeviceEntry>('connected_devices');
    if (!this.connectedDevicesCollection) {
      throw new LogicError("AccountsDatastoreCollection.onInitDbColleactions.2.");
    }
  }
  
  public insertConnectedDevice(accountId: number): DbConnectedDeviceEntry {
    let accEntry: DbAccountEntry = this.selectUserWithAccountId(accountId);
    if(accEntry == null) {
      throw new LogicError('Error.');
    }

    // Generate the random entry variables.
    let deviceId: number;
    let isDeviceIdFound: boolean = false;
    // Generate a account uid. Try 10 times.
    for(let i: number=0;i<50;i++) {
      // 1 000, 1 000 000 000
      let newDeviceId: number = this.rng.intBetween(1000, 1000000000);
      
      if(!this.existsConnectedDeviceWithDeviceId(newDeviceId)) {
        deviceId = newDeviceId;
        isDeviceIdFound = true;
        
        break;
      }
    }
    if(!isDeviceIdFound) {
      throw new LogicError('Error.');
    }

    let deviceKey: string = CryptoUtil.randomHexString(ClientsDatastoreConnectedDevices.DEVICE_KEY_STRING_LENGTH);
    
    // Push the device entry.
    let newEntry: DbConnectedDeviceEntry;
    
    try {
      newEntry = this.connectedDevicesCollection.insert(new DbConnectedDeviceEntry(
        accEntry.accountId,

        deviceId,
        deviceKey
      ));
    }catch(x) {
      throw x;
    }

    // Also update the client account connectedDevicesList.
    accEntry.connectedDevicesList.push(newEntry);
    this.userEntryChanged(accEntry);
    
    this.saveDatabase();

    return newEntry;
  }

  public selectConnectedDeviceWithDeviceId(deviceId: number): DbConnectedDeviceEntry {
    let results: DbConnectedDeviceEntry[] = <DbConnectedDeviceEntry[]><any>this.connectedDevicesCollection.find(
      {'deivceId': { '$eq' : deviceId }}
    );
    
    if(results.length == 0) {
      return null;
    }else if(results.length > 1) {
      throw new LogicError('AccountsDatastore.selectConnectedDeviceWithDeviceId.1.');
    }
    
    return results[0];
  }

  public existsConnectedDeviceWithDeviceId(deviceId: number): boolean {
    if(this.selectConnectedDeviceWithDeviceId(deviceId) != null) {
      return true;
    }
    
    return false;
  }

}
