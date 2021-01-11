"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWIExtendable_1 = require("./GWIExtendable");
class GWIExtenablePersistentIsolatedInstance extends GWIExtendable_1.GWIExtendable {
    constructor(handler, config) {
        super(handler, config);
        // This value is reseted after 5 seconds if no extra players added to it.
        this.possibleNewPlayersCount = 0;
        this.possibleNewPlayersTimoutS = 0;
        //void
    }
    getPossibleNewPlayersCount() {
        return this.possibleNewPlayersCount;
    }
    incrementPossibleNewPlayersCount() {
        this.possibleNewPlayersCount++;
        this.possibleNewPlayersTimoutS = GWIExtenablePersistentIsolatedInstance.POSSIBLE_NEW_PLAYERS_TIMEOUT_S;
    }
    appLoop_GameWorld(deltaS, deltaMS) {
        super.appLoop_GameWorld(deltaS, deltaMS);
        if (this.possibleNewPlayersCount > 0) {
            this.possibleNewPlayersTimoutS -= deltaS;
            if (this.possibleNewPlayersTimoutS <= 0) {
                this.possibleNewPlayersCount = 0;
            }
        }
    }
}
GWIExtenablePersistentIsolatedInstance.POSSIBLE_NEW_PLAYERS_TIMEOUT_S = 5;
exports.GWIExtenablePersistentIsolatedInstance = GWIExtenablePersistentIsolatedInstance;
//# sourceMappingURL=GWIExtenablePersistentIsolatedInstance.js.map