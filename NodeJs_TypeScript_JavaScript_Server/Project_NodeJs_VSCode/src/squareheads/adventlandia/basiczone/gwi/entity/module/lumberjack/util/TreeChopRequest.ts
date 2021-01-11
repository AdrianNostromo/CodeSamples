import { Pos3D } from "base/util/Pos3D";
import { EntityTemplate } from "localbase/gwi/entity/template/EntityTemplate";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export class TreeChopRequest {
  
  public treeEntityId: number;
  public treePos: Pos3D;
  public sortedDynamicVoxelIndex: number;
  public collectedItemTemplate: EntityTemplate;

  constructor(treeEntityId: number, treePos: Pos3D, sortedDynamicVoxelIndex: number, collectedItemTemplate: EntityTemplate) {
    this.treeEntityId = treeEntityId;
    this.treePos = treePos;
    this.sortedDynamicVoxelIndex = sortedDynamicVoxelIndex;
    this.collectedItemTemplate = collectedItemTemplate;
  }

  public computeByteSize(): number {
    let len: number = 0;

    len += 4;//treeEntityId
    len += 4 * 3;//treePos
    len += 4;//sortedDynamicVoxelIndex
    len += NetMsgsBlock.stringInBufferSize(this.collectedItemTemplate.id);//collectedItemId

    return len;
  }

  public writeToBuffer(buff: StreamBuffer): void {
    buff.writeInt(this.treeEntityId);

    buff.writeFloat(this.treePos.x);
    buff.writeFloat(this.treePos.y);
    buff.writeFloat(this.treePos.z);

    buff.writeInt(this.sortedDynamicVoxelIndex);
    
    buff.writeString(this.collectedItemTemplate.id);
  }

}
