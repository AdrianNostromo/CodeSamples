"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class GameClientType {
    constructor(id, name) {
        this.id = id;
        this.name = name;
        GameClientType.entriesList.push(this);
    }
    static fromID(typeID) {
        for (let i = 0; i < GameClientType.entriesList.length; i++) {
            let entry = GameClientType.entriesList[i];
            if (entry != null && entry.id == typeID) {
                return entry;
            }
        }
        return null;
    }
    static fromName(name) {
        for (let i = 0; i < GameClientType.entriesList.length; i++) {
            let entry = GameClientType.entriesList[i];
            if (entry != null && entry.name == name) {
                return entry;
            }
        }
        return null;
    }
}
GameClientType.entriesList = [];
GameClientType.SQUARE_HEADS = new GameClientType(1, 'SQUARE_HEADS');
exports.GameClientType = GameClientType;
//# sourceMappingURL=GameClientType.js.map