"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class ServerStatus {
    constructor(id, name, isTransientState) {
        this.id = id;
        this.name = name;
        this.isTransientState = isTransientState;
        ServerStatus.entriesList.push(this);
    }
    static fromID(typeID) {
        for (let i = 0; i < ServerStatus.entriesList.length; i++) {
            let entry = ServerStatus.entriesList[i];
            if (entry != null && entry.id == typeID) {
                return entry;
            }
        }
        return null;
    }
    static fromName(name) {
        for (let i = 0; i < ServerStatus.entriesList.length; i++) {
            let entry = ServerStatus.entriesList[i];
            if (entry != null && entry.name == name) {
                return entry;
            }
        }
        return null;
    }
}
ServerStatus.entriesList = [];
ServerStatus.OFFLINE = new ServerStatus(1, 'OFFLINE', false);
ServerStatus.ONLINE = new ServerStatus(4, 'ONLINE', false);
exports.ServerStatus = ServerStatus;
//# sourceMappingURL=ServerStatus.js.map