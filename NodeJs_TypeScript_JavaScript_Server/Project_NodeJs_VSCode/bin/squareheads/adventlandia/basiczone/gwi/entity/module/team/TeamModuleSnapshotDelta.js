"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var _a;
"use strict";
const TeamModuleBase_1 = require("./TeamModuleBase");
const BuilderModule_1 = require("../builder/BuilderModule");
const BooleanUtil_1 = require("base/util/BooleanUtil");
class TeamModuleSnapshotDelta extends TeamModuleBase_1.TeamModuleBase {
    constructor(entity, template) {
        super(entity, template);
        //void
    }
    onCurrentTeamIndexChanged() {
        super.onCurrentTeamIndexChanged();
        this.getEntity().setSnapshotDeltaFlag(TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.teamIndex);
    }
    onRcChange_joinNexusTeamResult() {
        super.onRcChange_joinNexusTeamResult();
        this.getEntity().setSnapshotDeltaFlag(TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.joinNexusTeamResultCode);
    }
    computeByteSize(flagsBitMap) {
        let len = super.computeByteSize(flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.teamIndex)) {
            len += 4;
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.joinNexusTeamResultCode)) {
            len += 4;
        }
        return len;
    }
    writeToBuffer(buff, flagsBitMap) {
        super.writeToBuffer(buff, flagsBitMap);
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.teamIndex)) {
            buff.writeInt(this.getTeamIndex());
        }
        if (BooleanUtil_1.BooleanUtil.is(flagsBitMap, TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.joinNexusTeamResultCode)) {
            buff.writeInt(this.getRc_joinNexusTeam());
        }
    }
    computeFullSyncFlagsBitMap(_flagsBitMap) {
        let flagsBitMap = super.computeFullSyncFlagsBitMap(_flagsBitMap);
        flagsBitMap = BooleanUtil_1.BooleanUtil.set(flagsBitMap, TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM.teamIndex);
        return flagsBitMap;
    }
}
TeamModuleSnapshotDelta.FLAGS_LOCAL_TEAM = (_a = class {
    },
    // ful, incremental
    _a.teamIndex = 1 + BuilderModule_1.BuilderModule.FLAGS_LOCAL_BUILDER.LAST_USED_BIT_INDEX,
    // incremental
    _a.joinNexusTeamResultCode = 2 + BuilderModule_1.BuilderModule.FLAGS_LOCAL_BUILDER.LAST_USED_BIT_INDEX,
    _a.LAST_USED_BIT_INDEX = 2 + BuilderModule_1.BuilderModule.FLAGS_LOCAL_BUILDER.LAST_USED_BIT_INDEX,
    _a);
exports.TeamModuleSnapshotDelta = TeamModuleSnapshotDelta;
//# sourceMappingURL=TeamModuleSnapshotDelta.js.map