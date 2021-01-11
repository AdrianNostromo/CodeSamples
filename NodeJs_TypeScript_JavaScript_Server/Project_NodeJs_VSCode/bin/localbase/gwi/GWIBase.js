"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicComponent_1 = require("base/component/BasicComponent");
const HandlerData_1 = require("./util/HandlerData");
const GlobalAccessLogger_1 = require("base/log/GlobalAccessLogger");
class GWIBase extends BasicComponent_1.BasicComponent {
    constructor(handler, config) {
        super();
        this._ILoopUserBase_GameWorld = true;
        this.isPlayEnabled = false;
        // Called when the match is no longer used.
        this.gameWorldAutoRemoveTimerS = -1;
        // Managed by handler.
        this.handlerData = new HandlerData_1.HandlerData();
        this.isInitFinished = false;
        this.handler = handler;
        this.config = config;
    }
    getIsInitFinished() {
        return this.isInitFinished;
    }
    getHandlerData() {
        return this.handlerData;
    }
    getConfig() {
        return this.config;
    }
    // extendable.
    checkIsInitFinished() {
        return true;
    }
    getIsPlayEnabled() {
        return this.isPlayEnabled;
    }
    setIsPlayEnabled(isPlayEnabled) {
        this.isPlayEnabled = isPlayEnabled;
    }
    appLoop_GameWorld(deltaS, deltaMS) {
        if (!this.isInitFinished) {
            if (this.checkIsInitFinished()) {
                this.isInitFinished = true;
            }
        }
        if (this.isPlayEnabled) {
            this.gameLoop_netInputs(deltaS, deltaMS);
            this.gameLoop_simulation(deltaS, deltaMS);
            this.gameLoop_entitiesRemoval(deltaS, deltaMS);
            this.gameLoop_sync(deltaS, deltaMS);
        }
    }
    gameLoop_netInputs(deltaS, deltaMS) {
        //void
    }
    gameLoop_simulation(deltaS, deltaMS) {
        //void
    }
    gameLoop_entitiesRemoval(deltaS, deltaMS) {
        //void
    }
    gameLoop_sync(deltaS, deltaMS) {
        //void
    }
    getServerInstanceId() {
        return this.config.serverInstanceId;
    }
    getScenarioId() {
        return this.config.scenarioId;
    }
    netError(msg) {
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug("GWIBase.netError.1: " + msg);
        if (this.handler) {
            this.handler.onNetError(this);
        }
    }
    logicError(msg) {
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug("GWIBase.logicError.1: " + msg);
        if (this.handler) {
            this.handler.onLogicError(this);
        }
    }
    activateAutoRemoveTimerS(timeS) {
        this.gameWorldAutoRemoveTimerS = timeS;
    }
    dispose() {
        if (this.handler != null) {
            this.handler = null;
        }
        super.dispose();
    }
}
exports.GWIBase = GWIBase;
//# sourceMappingURL=GWIBase.js.map