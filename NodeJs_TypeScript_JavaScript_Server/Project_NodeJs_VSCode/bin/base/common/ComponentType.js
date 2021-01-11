"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class ComponentType {
    constructor(id, name) {
        this.id = id;
        this.name = name;
        ComponentType.entriesList.push(this);
    }
    static fromID(typeID) {
        for (let i = 0; i < ComponentType.entriesList.length; i++) {
            let entry = ComponentType.entriesList[i];
            if (entry != null && entry.id == typeID) {
                return entry;
            }
        }
        return null;
    }
    static fromName(name) {
        for (let i = 0; i < ComponentType.entriesList.length; i++) {
            let entry = ComponentType.entriesList[i];
            if (entry != null && entry.name == name) {
                return entry;
            }
        }
        return null;
    }
}
ComponentType.entriesList = [];
ComponentType.SERVER_MANAGER = new ComponentType(1, 'SERVER_MANAGER');
ComponentType.NODE_CONTROLLER = new ComponentType(2, 'NODE_CONTROLLER');
ComponentType.LOG_MANAGER = new ComponentType(3, 'LOG_MANAGER');
ComponentType.SQUARE_HEADS_MANAGER = new ComponentType(5, 'SQUARE_HEADS_MANAGER');
exports.ComponentType = ComponentType;
//# sourceMappingURL=ComponentType.js.map