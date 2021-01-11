"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWISync_1 = require("./GWISync");
const SecurityError_1 = require("base/exception/SecurityError");
const GlobalAccessLogger_1 = require("base/log/GlobalAccessLogger");
const LogicError_1 = require("base/exception/LogicError");
class GWIPlayerActions extends GWISync_1.GWISync {
    constructor(handler, config) {
        super(handler, config);
        this.receivedPlayerActions = [];
        //void
    }
    onPlayerRemove_pre(pl) {
        super.onPlayerRemove_pre(pl);
        for (let i = 0; i < this.receivedPlayerActions.length; i++) {
            let action = this.receivedPlayerActions[i];
            if (!action.isProcessed && action.pl == pl) {
                action.isProcessed = true;
            }
        }
    }
    pushPlayerAction(pl, action) {
        action.pl = pl;
        this.receivedPlayerActions.push(action);
    }
    gameLoop_netInputs(deltaS, deltaMS) {
        super.gameLoop_netInputs(deltaS, deltaMS);
        if (this.receivedPlayerActions.length > 0) {
            for (let i = 0; i < this.receivedPlayerActions.length; i++) {
                let action = this.receivedPlayerActions[i];
                if (action.isProcessed) {
                    continue;
                }
                action.isProcessed = true;
                // Ignore action if the player is removed.
                if (action.pl.isRemoved) {
                    continue;
                }
                // Use exception handling and automatic player removal on certain exceptions.
                try {
                    this.processPlayerAction(action.pl, action);
                }
                catch (x) {
                    if (x instanceof SecurityError_1.SecurityError) {
                        GlobalAccessLogger_1.GlobalAccessLogger.getLog().security('Removing client. Security error: ' + x.message);
                        this.removePlayer(action.pl);
                    }
                    else if (x instanceof LogicError_1.LogicError) {
                        this.logicError("Error.processPlayerAction.1.");
                    }
                    else {
                        this.logicError("Error.processPlayerAction.1.");
                    }
                }
            }
            this.receivedPlayerActions = [];
        }
    }
}
exports.GWIPlayerActions = GWIPlayerActions;
//# sourceMappingURL=GWIPlayerActions.js.map