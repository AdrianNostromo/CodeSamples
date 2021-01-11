import { ZonesListInteractorBase } from "./ZonesListInteractorBase";
import { IAppLocal } from "squareheads/app/IAppLocal";
import { TcpServer } from "base/common/net/TcpServer";
import { IpPort } from "base/net/util/IpPort";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import * as net from "net";

export abstract class ZonesListInteractorNetListener extends ZonesListInteractorBase {

    private netClientsListener: TcpServer = null;
  
    public constructor(app: IAppLocal) {
        super(app);

        //void
    }
        
    protected create_framework(): void {
        super.create_framework();
        
        this.netClientsListener = new TcpServer(
            new IpPort(
                '127.0.0.1',
                0
            ),
            this.onNewUnvalidatedSocket.bind(this),
            this.onListenerNetError.bind(this),
            this.onListenerLogicError.bind(this),
            null
        );
        this.netClientsListener.create();
    }

    private onNewUnvalidatedSocket(target: TcpServer, socket: net.Socket): void {
        let con: GeneralTcpConnection = new GeneralTcpConnection(socket);

        this.onNewUnvalidatedCon(con);
    }

    protected abstract onNewUnvalidatedCon(con: GeneralTcpConnection): void;
    
    private onListenerNetError(target: TcpServer, error: Error): void {
        this.getApp().getLog().error('TcpServer. Error. Maybe the port is already in use.');

        this.getApp().getErrorHandler().error('TcpServer. Error. Maybe the port is already in use.');
    }

    private onListenerLogicError(target: TcpServer, error: Error): void {
        this.getApp().getLog().error('TcpServer. Error. Maybe the port is already in use.');

        this.getApp().getErrorHandler().error('TcpServer. Error. Maybe the port is already in use.');
    }

    public getClientsListenerIpPort(): IpPort {
        if(this.netClientsListener != null) {
            let ret: IpPort = this.netClientsListener.getIpPort();
            
            return ret;
        }
        
        return null;
    }

}
