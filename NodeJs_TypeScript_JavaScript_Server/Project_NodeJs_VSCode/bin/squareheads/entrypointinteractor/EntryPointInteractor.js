"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LocalAppComponent_1 = require("./../component/LocalAppComponent");
const HttpServer_1 = require("base/common/net/http/HttpServer");
const NetUtil_1 = require("base/common/net/util/NetUtil");
const GameClientType_1 = require("base/common/client/util/GameClientType");
const IpPort_1 = require("base/net/util/IpPort");
class EntryPointInteractor extends LocalAppComponent_1.LocalAppComponent {
    constructor(app) {
        super(app);
        this._httpServer = null;
        this._isListenerStarted = false;
        this.tIpPort = new IpPort_1.IpPort(null, 0);
        //void
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this._httpServer = new HttpServer_1.HttpServer(this.getApp(), 9002));
        this._httpServer.setCbRequest(this.onListenerRequest.bind(this));
    }
    componentStatusChanged(componentStatus) {
        super.componentStatusChanged(componentStatus);
        if (this.getApp().isStatus_STARTING()) {
            if (!this._isListenerStarted && this._httpServer) {
                this._isListenerStarted = true;
                this._httpServer.startListener();
            }
        }
    }
    onListenerRequest(request, response) {
        if (!this.getApp().isStatus_ONLINE_FOR_WORLD() || !this.getApp().isServerStatus_ONLINE()) {
            response.writeHead(503, { 'content-type': 'text/html' });
            response.end('HTTP/1.1 503 Service Unavailable\r\n\r\n');
            this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 6.');
            return;
        }
        if (request.url.indexOf('/favicon.ico') == 0) {
            this.writeBadRequest(response);
            this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 5.');
            return;
        }
        let ii = request.url.indexOf('/');
        if (request.url.indexOf('/') != 0) {
            this.writeBadRequest(response);
            this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 5.');
            return;
        }
        let decodedUrl = decodeURI(request.url);
        let client_type_name = NetUtil_1.NetUtil.extractParamValue('client_type_name', decodedUrl);
        let request_type = NetUtil_1.NetUtil.extractParamValue('request_type', decodedUrl);
        if (!client_type_name || !request_type) {
            this.writeBadRequest(response);
            this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 4.');
            return;
        }
        client_type_name = client_type_name.toUpperCase();
        //asdAA;// add the accountInteractor ipPort responder.
        let clientType = GameClientType_1.GameClientType.fromName(client_type_name);
        if (!clientType) {
            this.writeBadRequest(response);
            this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 3.');
            return;
        }
        else if (clientType == GameClientType_1.GameClientType.SQUARE_HEADS) {
            if (request_type == EntryPointInteractor.RT_component_ip_port) {
                let server_component_name = NetUtil_1.NetUtil.extractParamValue('server_component_name', decodedUrl);
                if (server_component_name == EntryPointInteractor.SCN_client_accounts_interactor) {
                    let ipPort = this.getApp().getSquareHeads().getClientAccountsInteractor().getClientsListenerIpPort();
                    if (!ipPort) {
                        this.getApp().getLog().warning('Http client request for client interactor ip port. Ip port is null. Critical error occured somewhere. Error is ignored at this location.');
                        this.netWrite500(response);
                        return;
                    }
                    // Recreate in case the received value contains extra information.
                    this.tIpPort.set(ipPort.ip, ipPort.port);
                    let s = JSON.stringify(this.tIpPort);
                    response.writeHead(200, { 'content-type': 'text/html' });
                    response.end(s);
                    this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - accounts_interactor_ip_port - ok.');
                }
                else {
                    this.writeBadRequest(response);
                    this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 1.');
                    return;
                }
            }
            else {
                this.writeBadRequest(response);
                this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 1.');
                return;
            }
        }
        else {
            this.writeBadRequest(response);
            this.getApp().getLog().debug('Entry point interactor. SQUARE_HEADS - bad request 2.');
            return;
        }
    }
    writeBadRequest(response) {
        response.writeHead(400, { 'content-type': 'text/html' });
        response.end('HTTP/1.1 400 Bad Request\r\n\r\n');
    }
    netWrite500(response) {
        // A server error occured. Don't do a fatal error, just end the connection.
        response.writeHead(500, { 'content-type': 'text/html' });
        response.end('HTTP/1.1 500 Internal Server Error\r\n\r\n');
    }
}
// RequestType
EntryPointInteractor.RT_component_ip_port = "component_ip_port";
// ServerComponentName
EntryPointInteractor.SCN_client_accounts_interactor = "client_accounts_interactor";
exports.EntryPointInteractor = EntryPointInteractor;
//# sourceMappingURL=EntryPointInteractor.js.map