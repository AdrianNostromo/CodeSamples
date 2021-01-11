"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class LogicException extends Error {
    constructor(message) {
        super(message);
        this.name = 'LogicException';
    }
}
exports.LogicException = LogicException;
//# sourceMappingURL=LogicException.js.map