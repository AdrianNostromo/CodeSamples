import { GWISyncNetHandling } from "./GWISyncNetHandling";
import { GameWorldSnapshotDelta } from "./sync/GameWorldSnapshotDelta";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { BasicPlayer } from "./util/BasicPlayer";
import { BasicWorldEntity } from "./entity/BasicWorldEntity";
import { LogicError } from "base/exception/LogicError";

export abstract class GWISync extends GWISyncNetHandling {

  protected snapshotDeltaIndex: number = 0;

  private gameWorldSnapshotDelta: GameWorldSnapshotDelta = new GameWorldSnapshotDelta();

  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }

  protected getGameWorldSnapshotDelta(): GameWorldSnapshotDelta {
    return this.gameWorldSnapshotDelta;
  }

  protected gameLoop_sync(deltaS: number, deltaMS: number): void {
    super.gameLoop_sync(deltaS, deltaMS);
    
    this.gameLoop_incrementalSync(deltaS, deltaMS);
    this.gameLoop_fullSync(deltaS, deltaMS);
  }

  private gameLoop_incrementalSync(deltaS: number, deltaMS: number): void {
    if(this.gameWorldSnapshotDelta.entitySnapshotDeltas.length > 0) {
      this.snapshotDeltaIndex++;

      // Generate the update byte buffer.
      let updateBuffer: StreamBuffer = this.gameWorldSnapshotDelta.generateDeltaUpdateBuffer();

      // Send the buffer to the players.
      this.sendSyncUpdateBufferToPlayers_partSync(updateBuffer.getBuffer());

      this.gameWorldSnapshotDelta.entitySnapshotDeltas = [];
    }
  }

  private gameLoop_fullSync(deltaS: number, deltaMS: number): void {
    if(this.isAnyPlayerRequireFullSync()) {
      let updateBuffer: StreamBuffer = this.generateFullUpdateBuffer();

      this.sendSyncUpdateBufferToPlayers_fullSync(updateBuffer.getBuffer());
    }
  }

  private isAnyPlayerRequireFullSync(): boolean {
    let playersList: BasicPlayer[] = this.getPlayersList();
    for(let i: number=0;i<playersList.length;i++) {
      let pl: BasicPlayer = playersList[i];
      if(pl != null && pl.getClient() != null && pl.getClient().lastSyncDeltaIndex == 0) {
        return true;
      }
    }

    return false;
  }

  public generateFullUpdateBuffer(): StreamBuffer {
    let dataLen: number = this.computeFullSyncUpdateBufferSize();

    let buff: StreamBuffer = new StreamBuffer(Buffer.allocUnsafe(dataLen));

    let entitiesList: BasicWorldEntity[] = this.getEntitiesList();

    buff.writeInt(entitiesList.length);

    for(let i: number=0;i<entitiesList.length;i++) {
      let entity: BasicWorldEntity = entitiesList[i];
      
      entity.writeToBuffer_fullSync(buff);
    }

    if(buff.getOffset() != buff.getBoundsLimit() || buff.getOffset() != buff.getLength()) {
      throw new LogicError('Error.');
    }

    return buff;
  }

  private computeFullSyncUpdateBufferSize(): number {
    let len: number = 0;

    // Entities count.
    len += 4;

    let entitiesList: BasicWorldEntity[] = this.getEntitiesList();
    for(let i: number=0;i<entitiesList.length;i++) {
      let entity: BasicWorldEntity = entitiesList[i];

      len += entity.computeByteSize_fullSync();
    }

    return len;
  }

  private sendSyncUpdateBufferToPlayers_partSync(updateBuffer: Buffer): void {
    let playersList: BasicPlayer[] = this.getPlayersList();
    for(let i: number=0;i<playersList.length;i++) {
      let pl: BasicPlayer = playersList[i];
      if(pl.getClient() != null && this.snapshotDeltaIndex - pl.getClient().lastSyncDeltaIndex == 1) {
        pl.getClient().lastSyncDeltaIndex = this.snapshotDeltaIndex;

        this.sendMsg_SC_C_SINK_GAME_WORLD_STATE(pl.getClient(), this.snapshotDeltaIndex, updateBuffer);
      }
    }
  }

  private sendSyncUpdateBufferToPlayers_fullSync(updateBuffer: Buffer): void {
    let playersList: BasicPlayer[] = this.getPlayersList();
    for(let i: number=0;i<playersList.length;i++) {
      let pl: BasicPlayer = playersList[i];
      if(pl.getClient() != null && pl.getClient().lastSyncDeltaIndex == 0) {
        pl.getClient().lastSyncDeltaIndex = this.snapshotDeltaIndex;

        this.sendMsg_SC_C_SINK_GAME_WORLD_STATE(pl.getClient(), this.snapshotDeltaIndex, updateBuffer);
      }
    }
  }

}
