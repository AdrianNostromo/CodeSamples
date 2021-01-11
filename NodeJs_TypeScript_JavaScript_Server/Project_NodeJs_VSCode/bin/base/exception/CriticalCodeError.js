"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class CriticalCodeError extends Error {
    constructor(message) {
        super(message);
        this.name = 'CriticalCodeError';
    }
}
exports.CriticalCodeError = CriticalCodeError;
//# sourceMappingURL=CriticalCodeError.js.map