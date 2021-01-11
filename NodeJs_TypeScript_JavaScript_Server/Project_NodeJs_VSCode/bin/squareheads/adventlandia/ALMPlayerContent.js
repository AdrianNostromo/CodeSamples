"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const AdventLandiaManagerBase_1 = require("./AdventLandiaManagerBase");
const PlayerContentDatastore_1 = require("./playercontentdatastore/PlayerContentDatastore");
const PlayerContentManager_1 = require("./playercontentmanager/PlayerContentManager");
const PlayerContentInteractor_1 = require("./playercontentinteractor/PlayerContentInteractor");
class ALMPlayerContent extends AdventLandiaManagerBase_1.AdventLandiaManagerBase {
    constructor(app) {
        super(app);
        this.playerContentDatastore = null;
        this.playerContentManager = null;
        this.playerContentInteractor = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.playerContentDatastore = new PlayerContentDatastore_1.PlayerContentDatastore(this.getApp()));
        this.addComponent(this.playerContentManager = new PlayerContentManager_1.PlayerContentManager(this.getApp()));
        this.addComponent(this.playerContentInteractor = new PlayerContentInteractor_1.PlayerContentInteractor(this.getApp()));
    }
    getPlayerContentDatastore() {
        return this.playerContentDatastore;
    }
    getPlayerContentManager() {
        return this.playerContentManager;
    }
    getPlayerContentInteractor() {
        return this.playerContentInteractor;
    }
}
exports.ALMPlayerContent = ALMPlayerContent;
//# sourceMappingURL=ALMPlayerContent.js.map