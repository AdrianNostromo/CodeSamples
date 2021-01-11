"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWISyncNetHandling_1 = require("./GWISyncNetHandling");
const GameWorldSnapshotDelta_1 = require("./sync/GameWorldSnapshotDelta");
const StreamBuffer_1 = require("base/buffer/StreamBuffer");
const LogicError_1 = require("base/exception/LogicError");
class GWISync extends GWISyncNetHandling_1.GWISyncNetHandling {
    constructor(handler, config) {
        super(handler, config);
        this.snapshotDeltaIndex = 0;
        this.gameWorldSnapshotDelta = new GameWorldSnapshotDelta_1.GameWorldSnapshotDelta();
        //void
    }
    getGameWorldSnapshotDelta() {
        return this.gameWorldSnapshotDelta;
    }
    gameLoop_sync(deltaS, deltaMS) {
        super.gameLoop_sync(deltaS, deltaMS);
        this.gameLoop_incrementalSync(deltaS, deltaMS);
        this.gameLoop_fullSync(deltaS, deltaMS);
    }
    gameLoop_incrementalSync(deltaS, deltaMS) {
        if (this.gameWorldSnapshotDelta.entitySnapshotDeltas.length > 0) {
            this.snapshotDeltaIndex++;
            // Generate the update byte buffer.
            let updateBuffer = this.gameWorldSnapshotDelta.generateDeltaUpdateBuffer();
            // Send the buffer to the players.
            this.sendSyncUpdateBufferToPlayers_partSync(updateBuffer.getBuffer());
            this.gameWorldSnapshotDelta.entitySnapshotDeltas = [];
        }
    }
    gameLoop_fullSync(deltaS, deltaMS) {
        if (this.isAnyPlayerRequireFullSync()) {
            let updateBuffer = this.generateFullUpdateBuffer();
            this.sendSyncUpdateBufferToPlayers_fullSync(updateBuffer.getBuffer());
        }
    }
    isAnyPlayerRequireFullSync() {
        let playersList = this.getPlayersList();
        for (let i = 0; i < playersList.length; i++) {
            let pl = playersList[i];
            if (pl != null && pl.getClient() != null && pl.getClient().lastSyncDeltaIndex == 0) {
                return true;
            }
        }
        return false;
    }
    generateFullUpdateBuffer() {
        let dataLen = this.computeFullSyncUpdateBufferSize();
        let buff = new StreamBuffer_1.StreamBuffer(Buffer.allocUnsafe(dataLen));
        let entitiesList = this.getEntitiesList();
        buff.writeInt(entitiesList.length);
        for (let i = 0; i < entitiesList.length; i++) {
            let entity = entitiesList[i];
            entity.writeToBuffer_fullSync(buff);
        }
        if (buff.getOffset() != buff.getBoundsLimit() || buff.getOffset() != buff.getLength()) {
            throw new LogicError_1.LogicError('Error.');
        }
        return buff;
    }
    computeFullSyncUpdateBufferSize() {
        let len = 0;
        // Entities count.
        len += 4;
        let entitiesList = this.getEntitiesList();
        for (let i = 0; i < entitiesList.length; i++) {
            let entity = entitiesList[i];
            len += entity.computeByteSize_fullSync();
        }
        return len;
    }
    sendSyncUpdateBufferToPlayers_partSync(updateBuffer) {
        let playersList = this.getPlayersList();
        for (let i = 0; i < playersList.length; i++) {
            let pl = playersList[i];
            if (pl.getClient() != null && this.snapshotDeltaIndex - pl.getClient().lastSyncDeltaIndex == 1) {
                pl.getClient().lastSyncDeltaIndex = this.snapshotDeltaIndex;
                this.sendMsg_SC_C_SINK_GAME_WORLD_STATE(pl.getClient(), this.snapshotDeltaIndex, updateBuffer);
            }
        }
    }
    sendSyncUpdateBufferToPlayers_fullSync(updateBuffer) {
        let playersList = this.getPlayersList();
        for (let i = 0; i < playersList.length; i++) {
            let pl = playersList[i];
            if (pl.getClient() != null && pl.getClient().lastSyncDeltaIndex == 0) {
                pl.getClient().lastSyncDeltaIndex = this.snapshotDeltaIndex;
                this.sendMsg_SC_C_SINK_GAME_WORLD_STATE(pl.getClient(), this.snapshotDeltaIndex, updateBuffer);
            }
        }
    }
}
exports.GWISync = GWISync;
//# sourceMappingURL=GWISync.js.map