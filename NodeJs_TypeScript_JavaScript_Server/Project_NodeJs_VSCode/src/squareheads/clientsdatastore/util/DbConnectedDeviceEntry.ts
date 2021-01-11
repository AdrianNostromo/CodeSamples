
export class DbConnectedDeviceEntry {

  public accountId: number;

  public deivceId: number;
  
  public deivceKey: string;

  public constructor(accountId: number, deivceId: number, deivceKey: string) {
    this.accountId = accountId;

    this.deivceId = deivceId;
    
    this.deivceKey = deivceKey;
  }

}
