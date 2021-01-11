"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class ServerConnectionCallbacksHolder {
    constructor(cbUnvalidatedConnection, cbConnectionFail, cbValidated) {
        this.cbUnvalidatedConnection = cbUnvalidatedConnection;
        this.cbConnectionFail = cbConnectionFail;
        this.cbValidated = cbValidated;
    }
}
ServerConnectionCallbacksHolder.uLogIdCounter = 0;
exports.ServerConnectionCallbacksHolder = ServerConnectionCallbacksHolder;
//# sourceMappingURL=ServerConnectionCallbacksHolder.js.map