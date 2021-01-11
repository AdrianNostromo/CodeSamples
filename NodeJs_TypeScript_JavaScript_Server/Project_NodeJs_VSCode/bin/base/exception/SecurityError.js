"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class SecurityError extends Error {
    constructor(message) {
        super(message);
        this.name = 'SecurityError';
    }
}
exports.SecurityError = SecurityError;
//# sourceMappingURL=SecurityError.js.map