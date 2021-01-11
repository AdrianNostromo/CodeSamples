"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class LogicError extends Error {
    constructor(message) {
        super(message);
        this.name = 'LogicError';
    }
}
exports.LogicError = LogicError;
//# sourceMappingURL=LogicError.js.map