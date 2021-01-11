"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var _a;
"use strict";
const LumberjackModuleBase_1 = require("./LumberjackModuleBase");
const VoxelInventoryModuleSnapshotDelta_1 = require("../voxelinventory/VoxelInventoryModuleSnapshotDelta");
const BooleanUtil_1 = require("base/util/BooleanUtil");
const LogicError_1 = require("base/exception/LogicError");
class LumberjackModuleSnapshotDelta extends LumberjackModuleBase_1.LumberjackModuleBase {
    constructor(entity, template) {
        super(entity, template);
        //void
    }
    throwChopAxe(treeEntityId, treePos, sortedDynamicVoxelIndex, collectedItemTemplate) {
        super.throwChopAxe(treeEntityId, treePos, sortedDynamicVoxelIndex, collectedItemTemplate);
        this.getEntity().setSnapshotDeltaFlag(LumberjackModuleSnapshotDelta.FLAGS_LOCAL_LUMBERJACK.chopTreeList);
    }
    computeByteSize(flagsBitMap) {
        let len = super.computeByteSize(flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, LumberjackModuleSnapshotDelta.FLAGS_LOCAL_LUMBERJACK.chopTreeList)) {
            let activeChopRequests = this.peekActiveChopRequests();
            if (activeChopRequests.length <= 0) {
                throw new LogicError_1.LogicError("LumberjackModuleSnapshotDelta.computeByteSize.1.");
            }
            len += 4; //activeChopRequests.length
            for (let i = 0; i < activeChopRequests.length; i++) {
                let entry = activeChopRequests[i];
                len += entry.computeByteSize();
            }
        }
        return len;
    }
    writeToBuffer(buff, flagsBitMap) {
        super.writeToBuffer(buff, flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, LumberjackModuleSnapshotDelta.FLAGS_LOCAL_LUMBERJACK.chopTreeList)) {
            let activeChopRequests = this.peekActiveChopRequests();
            if (activeChopRequests.length <= 0) {
                throw new LogicError_1.LogicError("LumberjackModuleSnapshotDelta.writeToBuffer.1.");
            }
            buff.writeInt(activeChopRequests.length);
            for (let i = 0; i < activeChopRequests.length; i++) {
                let entry = activeChopRequests[i];
                entry.writeToBuffer(buff);
            }
        }
    }
    computeFullSyncFlagsBitMap(_flagsBitMap) {
        let flagsBitMap = super.computeFullSyncFlagsBitMap(_flagsBitMap);
        // NOTE : Because the tree chop is instant on the server, don't do a tree chop sync on the initial full sync.
        /// The full sync will include the player inventory with the chopped tree voxels.
        /*if(this.peekActiveChopRequests().length > 0) {
          flagsBitMap = BooleanUtil.set(flagsBitMap, LumberjackModuleSnapshotDelta.FLAGS_LOCAL_LUMBERJACK.chopTreeList);
        }*/
        return flagsBitMap;
    }
}
//asdzz;// refactr the flags system to a better method that works with local, not statics.
LumberjackModuleSnapshotDelta.FLAGS_LOCAL_LUMBERJACK = (_a = class {
    },
    // i
    _a.chopTreeList = 1 + VoxelInventoryModuleSnapshotDelta_1.VoxelInventoryModuleSnapshotDelta.FLAGS_LOCAL_VOXEL_INVENTORY.LAST_USED_BIT_INDEX,
    _a.LAST_USED_BIT_INDEX = 1 + VoxelInventoryModuleSnapshotDelta_1.VoxelInventoryModuleSnapshotDelta.FLAGS_LOCAL_VOXEL_INVENTORY.LAST_USED_BIT_INDEX,
    _a);
exports.LumberjackModuleSnapshotDelta = LumberjackModuleSnapshotDelta;
//# sourceMappingURL=LumberjackModuleSnapshotDelta.js.map