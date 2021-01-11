"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const SHAdventLandia_1 = require("./SHAdventLandia");
const SolarOneManager_1 = require("./solarone/SolarOneManager");
class SHSolarOne extends SHAdventLandia_1.SHAdventLandia {
    constructor(app) {
        super(app);
        this.solarOne = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.solarOne = new SolarOneManager_1.SolarOneManager(this.getApp()));
    }
    getSolarOne() {
        return this.solarOne;
    }
}
exports.SHSolarOne = SHSolarOne;
//# sourceMappingURL=SHSolarOne.js.map