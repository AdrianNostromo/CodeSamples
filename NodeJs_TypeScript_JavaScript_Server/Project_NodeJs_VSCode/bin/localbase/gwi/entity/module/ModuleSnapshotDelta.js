"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var _a;
"use strict";
const ModuleBase_1 = require("./ModuleBase");
const BasicWorldEntity_1 = require("./../BasicWorldEntity");
class ModuleSnapshotDelta extends ModuleBase_1.ModuleBase {
    constructor(moduleId, entity, usesGameLoop) {
        super(moduleId, entity, usesGameLoop);
        //void
    }
    computeByteSize(flagsBitMap) {
        let len = 0;
        return 0;
    }
    writeToBuffer(buff, flagsBitMap) {
        //void
    }
    computeFullSyncFlagsBitMap(flagsBitMap) {
        return flagsBitMap;
    }
    syncFlagsCleared() {
        //void
    }
}
ModuleSnapshotDelta.FLAGS = (_a = class {
    },
    _a.LAST_USED_BIT_INDEX = 0 + BasicWorldEntity_1.BasicWorldEntity.FLAGS.LAST_USED_BIT_INDEX,
    _a);
exports.ModuleSnapshotDelta = ModuleSnapshotDelta;
//# sourceMappingURL=ModuleSnapshotDelta.js.map