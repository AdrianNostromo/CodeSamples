import { BasicAppComponent } from "base/component/BasicAppComponent";

import { IApp } from "base/app/IApp";

import * as http from "http";
import * as net from "net";

export class HttpServer extends BasicAppComponent {

  private _port: number;

  private _isStartRequested = false;

  private _httpListener: http.Server = null;

  private _cbRequest: (request: http.IncomingMessage, response: http.ServerResponse) => void = null;

  public constructor(app: IApp, port: number) {
    super(app);
    
    this._port = port;
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    this._httpListener = http.createServer(this.onRequest.bind(this));
    this._httpListener.on('clientError', this.onClientError.bind(this));
  }
  
  public startListener(): void {
    if(this._isStartRequested) {
      this.getApp().getLog().error('Exit. Initialize error 1.');

      this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
      
      return ;
    }
    
    this._isStartRequested = true;
    
    this._httpListener.listen(this._port);
  }
  
  private onClientError(err: Error, socket: net.Socket): void {
    //void
  }
  
  public setCbRequest(cbRequest: (request: http.IncomingMessage, response: http.ServerResponse) => void): void {
    this._cbRequest = cbRequest;
  }
  
  private onRequest(request: http.IncomingMessage, response: http.ServerResponse): void {
    if(!this._cbRequest) {
      this.getApp().getLog().error('Exit. Initialize error 1.');

      this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
      
      return ;
    }
    
    this._cbRequest(request, response);
  }
  
}
