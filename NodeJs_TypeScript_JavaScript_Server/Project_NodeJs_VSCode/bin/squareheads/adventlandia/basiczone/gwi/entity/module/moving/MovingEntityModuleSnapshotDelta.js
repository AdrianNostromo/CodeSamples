"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var _a;
"use strict";
const MovingEntityModuleBase_1 = require("./MovingEntityModuleBase");
const Module_1 = require("localbase/gwi/entity/module/Module");
const BooleanUtil_1 = require("base/util/BooleanUtil");
class MovingEntityModuleSnapshotDelta extends MovingEntityModuleBase_1.MovingEntityModuleBase {
    constructor(entity, template) {
        super(entity, template);
        //void
    }
    computeByteSize(flagsBitMap) {
        let len = super.computeByteSize(flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.move)) {
            len += 4 * 2;
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.stopMovement)) {
            //void
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.turn)) {
            len += 4;
        }
        return len;
    }
    writeToBuffer(buff, flagsBitMap) {
        super.writeToBuffer(buff, flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.move)) {
            buff.writeFloat(this.getDestMoveX());
            buff.writeFloat(this.getDestMoveY());
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.stopMovement)) {
            //void
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.turn)) {
            buff.writeFloat(this.getDdestTurnDZ());
        }
    }
    computeFullSyncFlagsBitMap(_flagsBitMap) {
        let flagsBitMap = super.computeFullSyncFlagsBitMap(_flagsBitMap);
        if (this.getIsMoveInProgress()) {
            flagsBitMap = BooleanUtil_1.BooleanUtil.set(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.move);
        }
        if (this.getIsMovementStopInProgress()) {
            flagsBitMap = BooleanUtil_1.BooleanUtil.set(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.stopMovement);
        }
        if (this.getIsTurnInProgress()) {
            flagsBitMap = BooleanUtil_1.BooleanUtil.set(flagsBitMap, MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.turn);
        }
        return flagsBitMap;
    }
    move(destMoveX, destMoveY) {
        super.move(destMoveX, destMoveY);
        this.getEntity().setSnapshotDeltaFlag(MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.move);
    }
    stopMovement() {
        super.stopMovement();
        this.getEntity().setSnapshotDeltaFlag(MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.stopMovement);
    }
    turn(destTurnDZ) {
        super.turn(destTurnDZ);
        this.getEntity().setSnapshotDeltaFlag(MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY.turn);
    }
}
MovingEntityModuleSnapshotDelta.FLAGS_LOCAL_MOVING_ENTITY = (_a = class {
    },
    // fo, i
    _a.move = 1 + Module_1.Module.FLAGS.LAST_USED_BIT_INDEX,
    // fo, i
    _a.stopMovement = 2 + Module_1.Module.FLAGS.LAST_USED_BIT_INDEX,
    // fo, i
    _a.turn = 3 + Module_1.Module.FLAGS.LAST_USED_BIT_INDEX,
    _a.LAST_USED_BIT_INDEX = 3 + Module_1.Module.FLAGS.LAST_USED_BIT_INDEX,
    _a);
exports.MovingEntityModuleSnapshotDelta = MovingEntityModuleSnapshotDelta;
//# sourceMappingURL=MovingEntityModuleSnapshotDelta.js.map