import { LocalAppComponent } from "./../component/LocalAppComponent";

import { IAppLocal } from "./../app/IAppLocal";

import { HttpServer } from "base/common/net/http/HttpServer";
import { NetUtil } from "base/common/net/util/NetUtil";
import { GameClientType } from "base/common/client/util/GameClientType";
import { ComponentStatus } from "base/common/util/ComponentStatus";
import * as http from "http";
import { IpPort } from "base/net/util/IpPort";

export class EntryPointInteractor extends LocalAppComponent {

  // RequestType
  private static RT_component_ip_port: string = "component_ip_port";
  // ServerComponentName
  private static SCN_client_accounts_interactor: string = "client_accounts_interactor";

  private _httpServer: HttpServer = null;
    
  private _isListenerStarted: boolean = false;

  public constructor(app: IAppLocal) {
    super(app);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    this.addComponent(this._httpServer = new HttpServer(this.getApp(), 9002));
    this._httpServer.setCbRequest(this.onListenerRequest.bind(this));
  }
  
  public componentStatusChanged(componentStatus: ComponentStatus): void {
    super.componentStatusChanged(componentStatus);
    
    if(this.getApp().isStatus_STARTING()) {
      if(!this._isListenerStarted && this._httpServer) {
        this._isListenerStarted = true;
        
        this._httpServer.startListener();
      }
    }
  }
  
  private tIpPort: IpPort = new IpPort(null, 0);
  private onListenerRequest(request: http.IncomingMessage, response: http.ServerResponse): void {
    if(!this.getApp().isStatus_ONLINE_FOR_WORLD() || !this.getApp().isServerStatus_ONLINE()) {
      response.writeHead(503, {'content-type': 'text/html'});
      response.end('HTTP/1.1 503 Service Unavailable\r\n\r\n');

      this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 6.');
      
      return ;
    }
    
    if(request.url.indexOf('/favicon.ico') == 0) {
      this.writeBadRequest(response);

      this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 5.');
      
      return ;
    }
    
    let ii: number = request.url.indexOf('/');
    if(request.url.indexOf('/') != 0) {
      this.writeBadRequest(response);

      this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 5.');
      
      return ;
    }
    
    let decodedUrl: string = decodeURI(request.url);
    
    let client_type_name: string = NetUtil.extractParamValue('client_type_name', decodedUrl);
    let request_type: string = NetUtil.extractParamValue('request_type', decodedUrl);
    if(!client_type_name || !request_type) {
      this.writeBadRequest(response);

      this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 4.');
      
      return ;
    }
    client_type_name = client_type_name.toUpperCase();

    //asdAA;// add the accountInteractor ipPort responder.

    let clientType: GameClientType = GameClientType.fromName(client_type_name);
    if(!clientType) {
      this.writeBadRequest(response);

      this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 3.');
      
      return ;

    }else if(clientType == GameClientType.SQUARE_HEADS) {
      if(request_type == EntryPointInteractor.RT_component_ip_port) {
        let server_component_name: string = NetUtil.extractParamValue('server_component_name', decodedUrl);

        if(server_component_name == EntryPointInteractor.SCN_client_accounts_interactor) {
          let ipPort: IpPort = this.getApp().getSquareHeads().getClientAccountsInteractor().getClientsListenerIpPort();
          if(!ipPort) {
            this.getApp().getLog().warning('Http client request for client interactor ip port. Ip port is null. Critical error occured somewhere. Error is ignored at this location.');
            
            this.netWrite500(response);
            
            return ;
          }
          
          // Recreate in case the received value contains extra information.
          this.tIpPort.set(
            ipPort.ip,
            ipPort.port
          );
          
          let s: string = JSON.stringify(this.tIpPort);
          
          response.writeHead(200, {'content-type': 'text/html'});
          response.end(s);

          this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - accounts_interactor_ip_port - ok.');
        }else {
          this.writeBadRequest(response);

          this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 1.');
          
          return ;
        }
      }else {
        this.writeBadRequest(response);

        this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 1.');
        
        return ;
      }
    }else {
      this.writeBadRequest(response);

      this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 2.');
      
      return ;
    }
  }

  private writeBadRequest(response: http.ServerResponse): void {
    response.writeHead(400, {'content-type': 'text/html'});
    response.end('HTTP/1.1 400 Bad Request\r\n\r\n');
  }
  

  private netWrite500(response: http.ServerResponse): void {
    // A server error occured. Don't do a fatal error, just end the connection.
    response.writeHead(500, {'content-type': 'text/html'});
    response.end('HTTP/1.1 500 Internal Server Error\r\n\r\n');
  }
  
}
