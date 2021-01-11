import * as net from "net";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export class BasicTcpConnection {

  protected _socket: net.Socket;
  
  private userDataMap: Map<string, Object> = new Map();

  protected _receivedNetBuffersList: StreamBuffer[] = [];

  private _cbConnect: (target: BasicTcpConnection) => void = null;
  private _cbClose: (target: BasicTcpConnection) => void = null;
  private _cbError: (target: BasicTcpConnection) => void = null;
  
  public constructor(socket: net.Socket) {
    this._socket = socket;
    
    // Bug workaround.
    // Simulate this._socket.setEncoding(null);
    if ((this._socket as any)._readableState) {
      delete (this._socket as any)._readableState.decoder
      delete (this._socket as any)._readableState.encoding
    }
    
    this._socket.on('connect', this.onSocketConnect.bind(this));
    this._socket.on('data', this.onSocketData.bind(this));
    this._socket.on('error', this.onSocketError.bind(this));
    this._socket.on('close', this.onSocketClose.bind(this));
  }
  
  private onSocketConnect(): void {
    if(this._cbConnect) {
      this._cbConnect(this);
    }
  }

  private onSocketData(data: Buffer): void {
    this.cacheInputBitesToBufferList(data);
  }
  
  private cacheInputBitesToBufferList(data: Buffer): void {
    let newBuff: StreamBuffer = new StreamBuffer(data);
    newBuff.setBoundsLimit(0);
    
    this._receivedNetBuffersList.push(newBuff);
  }
  
  private onSocketError(err: Error): void {
    if(this._cbError) {
      this._cbError(this);
    }
  }

  private onSocketClose(had_error: boolean): void {
    if(this._cbClose) {
      this._cbClose(this);
    }
  }
  
  public isData(): boolean {
    if(this._receivedNetBuffersList.length > 0) {
      return true;
    }
    
    return false;
  }

  public getUserData<T>(key: string): T {
    return <T>this.userDataMap.get(key);
  }

  public setUserData<T>(key: string, value: T): void {
    this.userDataMap.set(key, value);
  }

  public setCbConnect(cbConnect: (target: BasicTcpConnection) => void): void {
    this._cbConnect = cbConnect;
  }

  public setCbClose(cbClose: (target: BasicTcpConnection) => void): void {
    this._cbClose = cbClose;
  }

  public setCbError(cbError: (target: BasicTcpConnection) => void): void {
    this._cbError = cbError;
  }

  public getSocket(): net.Socket {
    return this._socket;
  }

  public forceClose(): void {
    if(!this._socket.destroyed) {
      this._socket.destroy();
    }
  }
  
}
