import { ClientsDatastoreConnectedDevices } from "./ClientsDatastoreConnectedDevices";

import { IAppLocal } from "./../app/IAppLocal";

import { DbClientSessionEntry } from "./util/DbClientSessionEntry";
import { LogicError } from "base/exception/LogicError";
import { DbAccountEntry } from "./util/DbAccountEntry";
import { CryptoUtil } from "base/crypto/util/CryptoUtil";

export abstract class ClientsDatastoreSessions extends ClientsDatastoreConnectedDevices {

  private static SESSION_KEY_STRING_LENGTH: number = 32;

  private sessionsCollection: Collection<DbClientSessionEntry> = null;

  public constructor(app: IAppLocal) {
    super(app);
    
    //void
  }

  protected factoryInitializeNewCollections(db: Loki): void {
    super.factoryInitializeNewCollections(db);

    db.addCollection<DbClientSessionEntry>('sessions', {
      indices: ['sessionId'],
      unique: ['sessionId']
    });
  }

  protected onInitDbColleactions(): void {
    super.onInitDbColleactions();
    
    this.sessionsCollection = this.dbInstance.getCollection<DbClientSessionEntry>('sessions');
    if (!this.sessionsCollection) {
      throw new LogicError("AccountsDatastoreCollection.onInitDbColleactions.3.");
    }
  }
  
  public insertSession(accountId: number): DbClientSessionEntry {
    let accEntry: DbAccountEntry = this.selectUserWithAccountId(accountId);
    if(accEntry == null) {
      throw new LogicError('Error.');
    }

    if(accEntry.activeSession != null) {
      throw new LogicError('Error.');
    }

    // Generate the random entry variables.
    let sessionId: number;
    let isSessionIdFound: boolean = false;
    // Generate a account uid. Try 10 times.
    for(let i: number=0;i<50;i++) {
      // 1 000, 1 000 000 000
      let newId: number = this.rng.intBetween(1000, 1000000000);
      
      if(!this.existsSessionWithSessionId(newId)) {
        sessionId = newId;
        isSessionIdFound = true;
        
        break;
      }
    }
    if(!isSessionIdFound) {
      throw new LogicError('Error.');
    }

    let sessionKey: string = CryptoUtil.randomHexString(ClientsDatastoreSessions.SESSION_KEY_STRING_LENGTH);
    
    // Push the session entry.
    let newEntry: DbClientSessionEntry;
    
    try {
      newEntry = this.sessionsCollection.insert(new DbClientSessionEntry(
        accEntry.accountId,

        sessionId,
        sessionKey
      ));
    }catch(x) {
      throw x;
    }

    // Also update the client account entry.
    accEntry.activeSession = newEntry;
    this.userEntryChanged(accEntry);
    
    this.saveDatabase();

    return newEntry;
  }

  public selectSessionWithSessionId(sessionId: number): DbClientSessionEntry {
    let results: DbClientSessionEntry[] = <DbClientSessionEntry[]><any>this.sessionsCollection.find(
      {'sessionId': { '$eq' : sessionId }}
    );
    
    if(results.length == 0) {
      return null;
    }else if(results.length > 1) {
      throw new LogicError('AccountsDatastore.selectSessionWithSessionId.1.');
    }
    
    return results[0];
  }

  public existsSessionWithSessionId(sessionId: number): boolean {
    if(this.selectSessionWithSessionId(sessionId) != null) {
      return true;
    }
    
    return false;
  }

}
