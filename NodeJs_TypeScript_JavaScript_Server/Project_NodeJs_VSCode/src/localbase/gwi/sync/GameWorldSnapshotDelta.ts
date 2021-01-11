import { BasicWorldEntitySnapshotDelta } from "./../entity/BasicWorldEntitySnapshotDelta";
import { LogicError } from "base/exception/LogicError";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export class GameWorldSnapshotDelta {

  public entitySnapshotDeltas: BasicWorldEntitySnapshotDelta[] = [];

  public constructor() {
    //void
  }

  public generateDeltaUpdateBuffer(): StreamBuffer {
    let dataLen: number = this.computePartSyncUpdateBufferSize();

    let buff: StreamBuffer = new StreamBuffer(Buffer.allocUnsafe(dataLen));
    
    buff.writeInt(this.entitySnapshotDeltas.length);
    
    for(let i: number=0;i<this.entitySnapshotDeltas.length;i++) {
      let eSnapshotDelta: BasicWorldEntitySnapshotDelta = this.entitySnapshotDeltas[i];
      
      eSnapshotDelta.writeToBuffer(buff);

      eSnapshotDelta.clearFlags();
    }
    this.entitySnapshotDeltas = [];

    if(buff.getOffset() != buff.getBoundsLimit() || buff.getOffset() != buff.getLength()) {
      throw new LogicError('Error.');
    }

    return buff;
  }

  private computePartSyncUpdateBufferSize(): number {
    let len: number = 0;

    // Entities count.
    len += 4;

    for(let i: number=0;i<this.entitySnapshotDeltas.length;i++) {
      let eSnapshotDelta: BasicWorldEntitySnapshotDelta = this.entitySnapshotDeltas[i];

      len += eSnapshotDelta.computeByteSize();
    }

    return len;
  }

}
