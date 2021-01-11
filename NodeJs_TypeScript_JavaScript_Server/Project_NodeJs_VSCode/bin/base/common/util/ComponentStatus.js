"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class ComponentStatus {
    constructor(id, name, isTransientState, isOnline) {
        this.id = id;
        this.name = name;
        this.isTransientState = isTransientState;
        this.isOnline = isOnline;
        ComponentStatus.entriesList.push(this);
    }
    static fromID(typeID) {
        for (let i = 0; i < ComponentStatus.entriesList.length; i++) {
            let entry = ComponentStatus.entriesList[i];
            if (entry != null && entry.id == typeID) {
                return entry;
            }
        }
        return null;
    }
    static fromName(name) {
        for (let i = 0; i < ComponentStatus.entriesList.length; i++) {
            let entry = ComponentStatus.entriesList[i];
            if (entry != null && entry.name == name) {
                return entry;
            }
        }
        return null;
    }
}
ComponentStatus.entriesList = [];
ComponentStatus.OFFLINE = new ComponentStatus(1, 'OFFLINE', false, false);
ComponentStatus.STOPING = new ComponentStatus(2, 'STOPING', true, false);
// Stating local modules.
ComponentStatus.STARTING = new ComponentStatus(3, 'STARTING', true, false);
// Establishes connections to other server components.
ComponentStatus.CONNECTING = new ComponentStatus(4, 'CONNECTING', true, false);
ComponentStatus.ONLINE_FOR_COMPONENTS = new ComponentStatus(5, 'ONLINE_FOR_COMPONENTS', true, true);
ComponentStatus.ONLINE_FOR_WORLD = new ComponentStatus(6, 'ONLINE_FOR_WORLD', false, true);
exports.ComponentStatus = ComponentStatus;
//# sourceMappingURL=ComponentStatus.js.map