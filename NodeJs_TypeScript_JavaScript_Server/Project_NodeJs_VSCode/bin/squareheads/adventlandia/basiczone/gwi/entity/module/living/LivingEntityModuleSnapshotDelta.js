"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var _a;
"use strict";
const LivingEntityModuleBase_1 = require("./LivingEntityModuleBase");
const MovingEntityModule_1 = require("../moving/MovingEntityModule");
const BooleanUtil_1 = require("base/util/BooleanUtil");
class LivingEntityModuleSnapshotDelta extends LivingEntityModuleBase_1.LivingEntityModuleBase {
    constructor(entity, template) {
        super(entity, template);
        //void
    }
    onCurrentHPChanged() {
        super.onCurrentHPChanged();
        // This value is not incrementaly synced.
        //this.getEntity().setSnapshotDeltaFlag(LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY.currentHP);
    }
    computeByteSize(flagsBitMap) {
        let len = super.computeByteSize(flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY.currentHP)) {
            len += 4;
        }
        return len;
    }
    writeToBuffer(buff, flagsBitMap) {
        super.writeToBuffer(buff, flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY.currentHP)) {
            buff.writeInt(this.getCurrentHP());
        }
    }
    computeFullSyncFlagsBitMap(_flagsBitMap) {
        let flagsBitMap = super.computeFullSyncFlagsBitMap(_flagsBitMap);
        flagsBitMap = BooleanUtil_1.BooleanUtil.set(flagsBitMap, LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY.currentHP);
        return flagsBitMap;
    }
}
LivingEntityModuleSnapshotDelta.FLAGS_LOCAL_LIVING_ENTITY = (_a = class {
    },
    // fm
    _a.currentHP = 1 + MovingEntityModule_1.MovingEntityModule.FLAGS_LOCAL_MOVING_ENTITY.LAST_USED_BIT_INDEX,
    _a.LAST_USED_BIT_INDEX = 1 + MovingEntityModule_1.MovingEntityModule.FLAGS_LOCAL_MOVING_ENTITY.LAST_USED_BIT_INDEX,
    _a);
exports.LivingEntityModuleSnapshotDelta = LivingEntityModuleSnapshotDelta;
//# sourceMappingURL=LivingEntityModuleSnapshotDelta.js.map