import { DbConnectedDeviceEntry } from "./DbConnectedDeviceEntry";
import { DbClientSessionEntry } from "./DbClientSessionEntry";

export class DbAccountEntry {

  public accountId: number;

  public socialUserId: number;
  
  public creationTimestampMs: number;
  
  public nickname: string;

  public connectedDevicesList: DbConnectedDeviceEntry[] = [];

  public activeSession: DbClientSessionEntry = null;

  public constructor(accountId: number, socialUserId: number, creationTimestampMs: number, nickname: string) {
    this.accountId = accountId;
    
    this.socialUserId = socialUserId;
    
    this.creationTimestampMs = creationTimestampMs;
    
    this.nickname = nickname;
  }

}
