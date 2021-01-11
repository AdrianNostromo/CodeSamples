"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LogicError_1 = require("base/exception/LogicError");
const StreamBuffer_1 = require("base/buffer/StreamBuffer");
class GameWorldSnapshotDelta {
    constructor() {
        this.entitySnapshotDeltas = [];
        //void
    }
    generateDeltaUpdateBuffer() {
        let dataLen = this.computePartSyncUpdateBufferSize();
        let buff = new StreamBuffer_1.StreamBuffer(Buffer.allocUnsafe(dataLen));
        buff.writeInt(this.entitySnapshotDeltas.length);
        for (let i = 0; i < this.entitySnapshotDeltas.length; i++) {
            let eSnapshotDelta = this.entitySnapshotDeltas[i];
            eSnapshotDelta.writeToBuffer(buff);
            eSnapshotDelta.clearFlags();
        }
        this.entitySnapshotDeltas = [];
        if (buff.getOffset() != buff.getBoundsLimit() || buff.getOffset() != buff.getLength()) {
            throw new LogicError_1.LogicError('Error.');
        }
        return buff;
    }
    computePartSyncUpdateBufferSize() {
        let len = 0;
        // Entities count.
        len += 4;
        for (let i = 0; i < this.entitySnapshotDeltas.length; i++) {
            let eSnapshotDelta = this.entitySnapshotDeltas[i];
            len += eSnapshotDelta.computeByteSize();
        }
        return len;
    }
}
exports.GameWorldSnapshotDelta = GameWorldSnapshotDelta;
//# sourceMappingURL=GameWorldSnapshotDelta.js.map