"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class GlobalAccessLogger {
    static getLog() {
        return GlobalAccessLogger.log;
    }
    static setLog(log) {
        GlobalAccessLogger.log = log;
    }
}
GlobalAccessLogger.log = null;
exports.GlobalAccessLogger = GlobalAccessLogger;
//# sourceMappingURL=GlobalAccessLogger.js.map