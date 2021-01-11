"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const SolarOneManagerBase_1 = require("./SolarOneManagerBase");
const ZonesListManager_1 = require("./zoneslistmanager/ZonesListManager");
const ZonesListInteractor_1 = require("./zoneslistinteractor/ZonesListInteractor");
class SOMZones extends SolarOneManagerBase_1.SolarOneManagerBase {
    constructor(app) {
        super(app);
        this.zonesListManager = null;
        this.zonesListInteractor = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.zonesListManager = new ZonesListManager_1.ZonesListManager(this.getApp()));
        this.addComponent(this.zonesListInteractor = new ZonesListInteractor_1.ZonesListInteractor(this.getApp()));
    }
}
exports.SOMZones = SOMZones;
//# sourceMappingURL=SOMZones.js.map