"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var _a;
"use strict";
const BasicWorldEntityBase_1 = require("./BasicWorldEntityBase");
const BooleanUtil_1 = require("base/util/BooleanUtil");
const LogicError_1 = require("base/exception/LogicError");
const StreamBuffer_1 = require("base/buffer/StreamBuffer");
const GlobalAccessLogger_1 = require("base/log/GlobalAccessLogger");
class BasicWorldEntitySnapshotDelta extends BasicWorldEntityBase_1.BasicWorldEntityBase {
    constructor(template, entityId, handler, gameWorldSnapshotDelta) {
        super(template, entityId, handler);
        this.flagsBitMap = 0;
        this.isEntityRemoved = false;
        this.gameWorldSnapshotDelta = gameWorldSnapshotDelta;
        this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.ownerPlayerId);
        this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.pos);
        this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.rot);
        this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.entityTemplateId);
    }
    clearFlags() {
        this.flagsBitMap = 0;
        for (let i = 0; i < this.modulesList.length; i++) {
            let entry = this.modulesList[i];
            entry.syncFlagsCleared();
        }
    }
    setSnapshotDeltaFlag(bitMapFlag) {
        if (this.flagsBitMap == 0) {
            this.gameWorldSnapshotDelta.entitySnapshotDeltas.push(this);
        }
        this.flagsBitMap = BooleanUtil_1.BooleanUtil.set(this.flagsBitMap, bitMapFlag);
    }
    computeByteSize(flagsBitMap = this.flagsBitMap) {
        let len = 0;
        // Entity id.
        len += 4;
        // The flags bit map.
        len += 4;
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.ownerPlayerId)) {
            len += 4;
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.pos)) {
            len += 4 * 3;
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.rot)) {
            len += 4 * 4;
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.entityRemoved)) {
            len += 0;
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.entityTemplateId)) {
            len += StreamBuffer_1.StreamBuffer.stringInBufferSize(this.template.id);
        }
        for (let i = 0; i < this.modulesList.length; i++) {
            let entry = this.modulesList[i];
            len += entry.computeByteSize(flagsBitMap);
        }
        return len;
    }
    writeToBuffer(buff, flagsBitMap = this.flagsBitMap) {
        buff.writeInt(this.entityId);
        if (flagsBitMap == 0) {
            throw new LogicError_1.LogicError('Error.');
        }
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug("Entity sync. entityId: " + this.entityId + ", flagsBitMap: " + flagsBitMap + ", template.id: " + this.template.id);
        buff.writeInt(flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.ownerPlayerId)) {
            if (this.getOwnerPlayer() != null) {
                buff.writeInt(this.getOwnerPlayer().playerId);
            }
            else {
                buff.writeInt(0);
            }
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.pos)) {
            buff.writeFloat(this.getPos().x);
            buff.writeFloat(this.getPos().y);
            buff.writeFloat(this.getPos().z);
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.rot)) {
            buff.writeFloat(this.getRot().x);
            buff.writeFloat(this.getRot().y);
            buff.writeFloat(this.getRot().z);
            buff.writeFloat(this.getRot().w);
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.entityRemoved)) {
            //void
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.entityTemplateId)) {
            buff.writeString(this.template.id);
        }
        for (let i = 0; i < this.modulesList.length; i++) {
            let entry = this.modulesList[i];
            entry.writeToBuffer(buff, flagsBitMap);
        }
    }
    computeFullSyncFlagsBitMap(flagsBitMap) {
        flagsBitMap = BooleanUtil_1.BooleanUtil.set(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.ownerPlayerId);
        flagsBitMap = BooleanUtil_1.BooleanUtil.set(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.pos);
        flagsBitMap = BooleanUtil_1.BooleanUtil.set(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.rot);
        flagsBitMap = BooleanUtil_1.BooleanUtil.set(flagsBitMap, BasicWorldEntitySnapshotDelta.FLAGS.entityTemplateId);
        for (let i = 0; i < this.modulesList.length; i++) {
            let entry = this.modulesList[i];
            flagsBitMap = entry.computeFullSyncFlagsBitMap(flagsBitMap);
        }
        return flagsBitMap;
    }
    computeByteSize_fullSync() {
        let flagsBitMap = this.computeFullSyncFlagsBitMap(0);
        return this.computeByteSize(flagsBitMap);
    }
    writeToBuffer_fullSync(buff) {
        let flagsBitMap = this.computeFullSyncFlagsBitMap(0);
        this.writeToBuffer(buff, flagsBitMap);
    }
    onOwnerPlayerChanged() {
        super.onOwnerPlayerChanged();
        this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.ownerPlayerId);
    }
    onPosChanged() {
        super.onPosChanged();
        this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.pos);
    }
    onRotDChanged() {
        super.onRotDChanged();
        this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.rot);
    }
    onEntityRemove_pre() {
        super.onEntityRemove_pre();
        this.isEntityRemoved = true;
        this.setSnapshotDeltaFlag(BasicWorldEntitySnapshotDelta.FLAGS.entityRemoved);
    }
}
BasicWorldEntitySnapshotDelta.FLAGS = (_a = class {
    },
    // fm
    _a.ownerPlayerId = 0,
    // fm
    _a.pos = 1,
    // fm
    _a.rot = 2,
    // i
    _a.entityRemoved = 3,
    // fm
    _a.entityTemplateId = 4,
    _a.LAST_USED_BIT_INDEX = 4,
    _a);
exports.BasicWorldEntitySnapshotDelta = BasicWorldEntitySnapshotDelta;
//# sourceMappingURL=BasicWorldEntitySnapshotDelta.js.map