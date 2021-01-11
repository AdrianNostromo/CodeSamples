"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var _a;
"use strict";
const BuilderModuleBase_1 = require("./BuilderModuleBase");
const LumberjackModule_1 = require("../lumberjack/LumberjackModule");
const BooleanUtil_1 = require("base/util/BooleanUtil");
class BuilderModuleSnapshotDelta extends BuilderModuleBase_1.BuilderModuleBase {
    constructor(entity, template) {
        super(entity, template);
        //void
    }
    onRcChange_buildNexusAndJoinTeamResult() {
        super.onRcChange_buildNexusAndJoinTeamResult();
        this.getEntity().setSnapshotDeltaFlag(BuilderModuleSnapshotDelta.FLAGS_LOCAL_BUILDER.buildNexusAndJoinTeamResultCode);
    }
    computeByteSize(flagsBitMap) {
        let len = super.computeByteSize(flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, BuilderModuleSnapshotDelta.FLAGS_LOCAL_BUILDER.buildNexusAndJoinTeamResultCode)) {
            //void
        }
        return len;
    }
    writeToBuffer(buff, flagsBitMap) {
        super.writeToBuffer(buff, flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, BuilderModuleSnapshotDelta.FLAGS_LOCAL_BUILDER.buildNexusAndJoinTeamResultCode)) {
            //void
        }
    }
    computeFullSyncFlagsBitMap(_flagsBitMap) {
        let flagsBitMap = super.computeFullSyncFlagsBitMap(_flagsBitMap);
        //void
        return flagsBitMap;
    }
}
BuilderModuleSnapshotDelta.FLAGS_LOCAL_BUILDER = (_a = class {
    },
    // incremental
    _a.buildNexusAndJoinTeamResultCode = 2 + LumberjackModule_1.LumberjackModule.FLAGS_LOCAL_LUMBERJACK.LAST_USED_BIT_INDEX,
    _a.LAST_USED_BIT_INDEX = 2 + LumberjackModule_1.LumberjackModule.FLAGS_LOCAL_LUMBERJACK.LAST_USED_BIT_INDEX,
    _a);
exports.BuilderModuleSnapshotDelta = BuilderModuleSnapshotDelta;
//# sourceMappingURL=BuilderModuleSnapshotDelta.js.map