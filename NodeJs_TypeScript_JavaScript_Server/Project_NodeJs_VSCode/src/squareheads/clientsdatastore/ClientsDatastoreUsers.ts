import { ClientsDatastoreBase } from "./ClientsDatastoreBase";

import { IAppLocal } from "./../app/IAppLocal";

import { DbAccountEntry } from "./util/DbAccountEntry";
import { LogicError } from "base/exception/LogicError";

export abstract class ClientsDatastoreUsers extends ClientsDatastoreBase {

  private usersCollection: Collection<DbAccountEntry> = null;

  public constructor(app: IAppLocal) {
    super(app);
    
    //void
  }

  protected factoryInitializeNewCollections(db: Loki): void {
    super.factoryInitializeNewCollections(db);

    db.addCollection<DbAccountEntry>('users', {
      indices: ['accountId', 'socialUserId', 'nickname'],
      unique: ['accountId', 'socialUserId']
    });
  }

  protected onInitDbColleactions(): void {
    super.onInitDbColleactions();
    
    this.usersCollection = this.dbInstance.getCollection<DbAccountEntry>('users');
    if (!this.usersCollection) {
      throw new LogicError("AccountsDatastoreCollection.onInitDbColleactions.1.");
    }
  }
  
  protected userEntryChanged(accEntry: DbAccountEntry): void {
    this.usersCollection.update(accEntry);
  }

  public selectUserWithAccountId(sanitized_accountId: number): DbAccountEntry {
    let results: DbAccountEntry[] = <DbAccountEntry[]><any>this.usersCollection.find(
      {'accountId': { '$eq' : sanitized_accountId }}
    );
    
    if(results.length == 0) {
      return null;
    }else if(results.length > 1) {
      throw new LogicError('AccountsDatastore.selectUserWithAccountId.1.');
    }
    
    return results[0];
  }
  
  public existsUserWithAccountId(sanitized_accountId: number): boolean {
    if(this.selectUserWithAccountId(sanitized_accountId) != null) {
      return true;
    }
    
    return false;
  }

  public updateAccNicknameWithAccountId(sanitized_accountId: number, sanitized_nickname: string) {
    let entry: DbAccountEntry = this.selectUserWithAccountId(sanitized_accountId);
    if(entry && entry.nickname != sanitized_nickname) {
      entry.nickname = sanitized_nickname;
      
      this.usersCollection.update(entry);

      this.saveDatabase();
    }
    
    return entry;
  }
  
  public selectUserWithSocialUserId(sanitized_socialUserId: number): DbAccountEntry {
    let results: DbAccountEntry[] = <DbAccountEntry[]><any>this.usersCollection.find({'socialUserId': { '$eq' : sanitized_socialUserId }});
    
    if(results.length == 0) {
      return null;
    }else if(results.length > 1) {
      throw new LogicError('AccountsDatastore.selectSingleUserWithSocialUserId.1.');
    }
    
    return results[0];
  }
  
  public existsUserWithSocialUserId(sanitized_socialUserId: number): boolean {
    if(this.selectUserWithSocialUserId(sanitized_socialUserId) != null) {
      return true;
    }
    
    return false;
  }

  public insertAccount(sanitized_nickname: string): DbAccountEntry {
    let accountId: number;
    let isAccountIdFound: boolean = false;
    // Generate a account uid. Try 10 times.
    for(let i: number=0;i<50;i++) {
      // 1 000, 1 000 000 000
      let newAccountId: number = this.rng.intBetween(1000, 1000000000);
      
      if(!this.existsUserWithAccountId(newAccountId)) {
        accountId = newAccountId;
        isAccountIdFound = true;
        
        break;
      }
    }
    if(!isAccountIdFound) {
      throw new LogicError('Error.');
    }

    let socialUserId: number;
    let isSocialUserIdFound: boolean = false;
    // Generate a account uid. Try 10 times.
    for(let i: number=0;i<50;i++) {
      let newSocialUserId: number = this.rng.intBetween(1000, 1000000000);
      
      if(!this.existsUserWithSocialUserId(newSocialUserId)) {
        socialUserId = newSocialUserId;
        isSocialUserIdFound = true;
        
        break;
      }
    }
    if(!isSocialUserIdFound) {
      throw new LogicError('Error.');
    }

    let newEntry: DbAccountEntry = null;
    try {
      newEntry = this.usersCollection.insert(new DbAccountEntry(
        accountId,
        socialUserId,

        (new Date()).getTime(),

        sanitized_nickname
      ));
    }catch(x) {
      throw x;
    }
    if(!newEntry) {
      throw new LogicError('Error.');
    }

    this.saveDatabase();
    
    return newEntry;
  }
  
}
