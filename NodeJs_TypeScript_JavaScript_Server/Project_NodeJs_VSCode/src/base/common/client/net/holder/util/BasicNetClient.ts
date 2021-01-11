import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";

export class BasicNetClient {

  private con: GeneralTcpConnection = null;

  public receiveIddleTimeS: number = 0;
  
  // if >= 0; Connection will be removed on timeout expiration.
  public forceDisconnectTimeoutS: number = -1;

  public markForRemoval: boolean = false;

  // The same behaviour as above but used by a different mechanic.
  public currentCommandTimeoutS: number = -1;

  public isPingAllowed: boolean = false;
  public isPingInProgress: boolean = false;
  public activePingTimeoutS: number;

  public remotePort: number;
  
  constructor(con: GeneralTcpConnection) {
    if(con != null) {
      this.setCon(con);
    }
  }

  public getCon(): GeneralTcpConnection {
    return this.con;
  }

  // Thic can be used to extract the con and using it somewhere else without closing it.
  public setCon(con: GeneralTcpConnection): void {
    if(this.con != null) {
      this.con.setCbClose(null);
      this.con.setCbError(null);

      if(this.con.getUserData<BasicNetClient>("client") != this) {
        this.con.setUserData<BasicNetClient>("client", null);
      }

      this.con = null;
    }

    this.con = con;
    if(this.con != null) {
      this.con.setUserData<BasicNetClient>("client", this);
      this.remotePort = this.con.getSocket().remotePort;
    }
  }
  
}
