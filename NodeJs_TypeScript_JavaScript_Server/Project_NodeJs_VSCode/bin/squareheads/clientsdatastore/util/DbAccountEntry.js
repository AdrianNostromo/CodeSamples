"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class DbAccountEntry {
    constructor(accountId, socialUserId, creationTimestampMs, nickname) {
        this.connectedDevicesList = [];
        this.activeSession = null;
        this.accountId = accountId;
        this.socialUserId = socialUserId;
        this.creationTimestampMs = creationTimestampMs;
        this.nickname = nickname;
    }
}
exports.DbAccountEntry = DbAccountEntry;
//# sourceMappingURL=DbAccountEntry.js.map