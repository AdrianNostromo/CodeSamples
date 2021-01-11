
export class DbClientSessionEntry {

  public accountId: number;

  public sessionId: number;

  public sessionKey: string;

  public constructor(accountId: number, sessionId: number, sessionKey: string) {
    this.accountId = accountId;
    
    this.sessionId = sessionId;
    
    this.sessionKey = sessionKey;
  }

}
