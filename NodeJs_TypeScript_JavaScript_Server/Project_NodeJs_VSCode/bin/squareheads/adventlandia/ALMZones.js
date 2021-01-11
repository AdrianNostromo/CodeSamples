"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ZonesListManager_1 = require("./zoneslistmanager/ZonesListManager");
const ZonesListInteractor_1 = require("./zoneslistinteractor/ZonesListInteractor");
const ALMPlayerContent_1 = require("./ALMPlayerContent");
class ALMZones extends ALMPlayerContent_1.ALMPlayerContent {
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
    getZonesListManager() {
        return this.zonesListManager;
    }
    getZonesListInteractor() {
        return this.zonesListInteractor;
    }
}
exports.ALMZones = ALMZones;
//# sourceMappingURL=ALMZones.js.map