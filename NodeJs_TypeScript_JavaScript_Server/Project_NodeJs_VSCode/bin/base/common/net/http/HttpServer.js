"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
const http = require("http");
class HttpServer extends BasicAppComponent_1.BasicAppComponent {
    constructor(app, port) {
        super(app);
        this._isStartRequested = false;
        this._httpListener = null;
        this._cbRequest = null;
        this._port = port;
    }
    create_framework() {
        super.create_framework();
        this._httpListener = http.createServer(this.onRequest.bind(this));
        this._httpListener.on('clientError', this.onClientError.bind(this));
    }
    startListener() {
        if (this._isStartRequested) {
            this.getApp().getLog().error('Exit. Initialize error 1.');
            this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
            return;
        }
        this._isStartRequested = true;
        this._httpListener.listen(this._port);
    }
    onClientError(err, socket) {
        //void
    }
    setCbRequest(cbRequest) {
        this._cbRequest = cbRequest;
    }
    onRequest(request, response) {
        if (!this._cbRequest) {
            this.getApp().getLog().error('Exit. Initialize error 1.');
            this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
            return;
        }
        this._cbRequest(request, response);
    }
}
exports.HttpServer = HttpServer;
//# sourceMappingURL=HttpServer.js.map