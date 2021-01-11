"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
const GlobalAccessLogger_1 = require("./GlobalAccessLogger");
class Logger extends BasicAppComponent_1.BasicAppComponent {
    constructor(app) {
        super(app);
        this._ILoopUserBase_General = true;
        this._delayMonitor = new Map();
        this._currentTimeS = 0;
        GlobalAccessLogger_1.GlobalAccessLogger.setLog(this);
    }
    create_framework() {
        super.create_framework();
        //void
    }
    appLoop_General(deltaS, deltaMS) {
        this._currentTimeS += deltaS;
    }
    getNextULogId() {
        return ++Logger.uLogIdCounter;
    }
    debug(msg, msgId, minDelayS) {
        this.pushLog('d> ', msg, msgId, minDelayS);
    }
    info(msg, msgId, minDelayS) {
        this.pushLog('i> ', msg, msgId, minDelayS);
    }
    console(msg, msgId, minDelayS) {
        this.pushLog('c> ', msg, msgId, minDelayS);
    }
    warning(msg, msgId, minDelayS) {
        this.pushLog('w> ', msg, msgId, minDelayS);
    }
    error(msg, msgId, minDelayS) {
        this.pushLog('e> ', msg, msgId, minDelayS);
    }
    security(msg, msgId, minDelayS) {
        this.pushLog('s> ', msg, msgId, minDelayS);
    }
    securityWarning(msg, msgId, minDelayS) {
        this.pushLog('sw> ', msg, msgId, minDelayS);
    }
    pushLog(prefix, msg, msgId, minDelayS) {
        if (msgId !== undefined && msgId !== null && minDelayS !== undefined && minDelayS !== null) {
            if (!this._delayMonitor.has(msgId) || this._delayMonitor.get(msgId) + minDelayS <= this._currentTimeS) {
                // Post it;
                this._delayMonitor.set(msgId, this._currentTimeS);
            }
            else {
                return;
            }
        }
        console.log(prefix + msg);
    }
}
Logger.uLogIdCounter = 0;
exports.Logger = Logger;
//# sourceMappingURL=Logger.js.map