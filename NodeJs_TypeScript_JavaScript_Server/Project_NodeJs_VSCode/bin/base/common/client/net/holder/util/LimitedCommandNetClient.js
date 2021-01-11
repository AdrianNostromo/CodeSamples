"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicNetClient_1 = require("./BasicNetClient");
const LogicError_1 = require("base/exception/LogicError");
class LimitedCommandNetClient extends BasicNetClient_1.BasicNetClient {
    constructor(con) {
        super(con);
        // -1: no command.
        // NOTE : the currentCommandTimeoutS must be set to -1 even if there is no active command.
        this.currentCommandNetCode = -1;
        //void
    }
    getCurrentCommandNetCode() {
        return this.currentCommandNetCode;
    }
    setCurrentCommandNetCode(currentCommandNetCode) {
        if (this.currentCommandNetCode != -1) {
            throw new LogicError_1.LogicError('Error.');
        }
        this.currentCommandNetCode = currentCommandNetCode;
        this.currentCommandTimeoutS = LimitedCommandNetClient.DEFAULT_COMMAND_TIMEOUT_S;
    }
    clearCurrentCommandNetCode() {
        this.currentCommandNetCode = -1;
        this.currentCommandTimeoutS = -1;
    }
}
LimitedCommandNetClient.DEFAULT_COMMAND_TIMEOUT_S = 5;
exports.LimitedCommandNetClient = LimitedCommandNetClient;
//# sourceMappingURL=LimitedCommandNetClient.js.map