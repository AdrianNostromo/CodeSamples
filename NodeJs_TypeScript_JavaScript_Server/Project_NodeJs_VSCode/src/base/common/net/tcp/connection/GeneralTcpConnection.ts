import { TcpCryptoConnection } from './TcpCryptoConnection';

import * as net from "net";

export class GeneralTcpConnection extends TcpCryptoConnection {

  public constructor(socket: net.Socket) {
    super(socket);
    
    //void
  }
  
}
