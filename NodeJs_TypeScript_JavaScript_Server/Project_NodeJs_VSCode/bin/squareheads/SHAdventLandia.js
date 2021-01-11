"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const SquareHeadsManagerUsers_1 = require("./SquareHeadsManagerUsers");
const AdventLandiaManager_1 = require("./adventlandia/AdventLandiaManager");
class SHAdventLandia extends SquareHeadsManagerUsers_1.SquareHeadsManagerUsers {
    constructor(app) {
        super(app);
        this.adventLandia = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.adventLandia = new AdventLandiaManager_1.AdventLandiaManager(this.getApp()));
    }
    getAdventLandia() {
        return this.adventLandia;
    }
}
exports.SHAdventLandia = SHAdventLandia;
//# sourceMappingURL=SHAdventLandia.js.map