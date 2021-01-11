export class ClientSessionInfo {

  public id: number;
  public key: string;

  public accountId: number;

  public constructor(id: number, key: string, accountId: number) {
    this.id = id;
    this.key = key;

    this.accountId = accountId;
  }

}
