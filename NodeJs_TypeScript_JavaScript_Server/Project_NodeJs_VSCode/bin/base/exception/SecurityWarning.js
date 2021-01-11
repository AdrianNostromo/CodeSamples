"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class SecurityWarning extends Error {
    constructor(message) {
        super(message);
        this.name = 'SecurityWarning';
    }
}
exports.SecurityWarning = SecurityWarning;
//# sourceMappingURL=SecurityWarning.js.map