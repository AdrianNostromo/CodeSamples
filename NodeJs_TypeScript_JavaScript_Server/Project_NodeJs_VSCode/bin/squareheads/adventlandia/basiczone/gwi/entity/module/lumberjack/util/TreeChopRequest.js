"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
class TreeChopRequest {
    constructor(treeEntityId, treePos, sortedDynamicVoxelIndex, collectedItemTemplate) {
        this.treeEntityId = treeEntityId;
        this.treePos = treePos;
        this.sortedDynamicVoxelIndex = sortedDynamicVoxelIndex;
        this.collectedItemTemplate = collectedItemTemplate;
    }
    computeByteSize() {
        let len = 0;
        len += 4; //treeEntityId
        len += 4 * 3; //treePos
        len += 4; //sortedDynamicVoxelIndex
        len += NetMsgsBlock_1.NetMsgsBlock.stringInBufferSize(this.collectedItemTemplate.id); //collectedItemId
        return len;
    }
    writeToBuffer(buff) {
        buff.writeInt(this.treeEntityId);
        buff.writeFloat(this.treePos.x);
        buff.writeFloat(this.treePos.y);
        buff.writeFloat(this.treePos.z);
        buff.writeInt(this.sortedDynamicVoxelIndex);
        buff.writeString(this.collectedItemTemplate.id);
    }
}
exports.TreeChopRequest = TreeChopRequest;
//# sourceMappingURL=TreeChopRequest.js.map