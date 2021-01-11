import { ClientSessionsManagerBase } from "./ClientSessionsManagerBase";

import { IAppLocal } from "./../app/IAppLocal";

import { ClientSessionInfo } from "./util/ClientSessionInfo";
import { DbClientSessionEntry } from "squareheads/clientsdatastore/util/DbClientSessionEntry";
import { DbAccountEntry } from "squareheads/clientsdatastore/util/DbAccountEntry";

export class ClientSessionsManager extends ClientSessionsManagerBase {

  public constructor(app: IAppLocal) {
    super(app);
    
    //void
  }

  public getOrCreateClientSessionWithAccId(accountId: number): ClientSessionInfo {
    // Get the account entry.
    let accEntry: DbAccountEntry = this.getApp().getSquareHeads().getClientsDatastore().selectUserWithAccountId(accountId);

    if(accEntry.activeSession == null) {
      // If no session, create one.
      this.getApp().getSquareHeads().getClientsDatastore().insertSession(accountId);
    }

    let clientSessionEntry: DbClientSessionEntry = accEntry.activeSession;
    
    let ret: ClientSessionInfo = new ClientSessionInfo(
      clientSessionEntry.sessionId,
      clientSessionEntry.sessionKey,

      clientSessionEntry.accountId
    );

    return ret;
  }
  
}
