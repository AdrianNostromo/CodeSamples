"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const MoveAction_1 = require("./MoveAction");
const StopMovementAction_1 = require("./StopMovementAction");
const TurnAction_1 = require("./TurnAction");
const ChopTreeAction_1 = require("./ChopTreeAction");
const BuildNexusByUnitAction_1 = require("./BuildNexusByUnitAction");
const JoinNexusTeamByUnitAction_1 = require("./JoinNexusTeamByUnitAction");
const LogicError_1 = require("base/exception/LogicError");
const Pos3D_1 = require("base/util/Pos3D");
class ActionSerializer {
    /**
     *
     * @param buff
     * @throws Error
     */
    static readFromBuffer(buff) {
        let type = buff.readInt();
        let action = null;
        switch (type) {
            case MoveAction_1.MoveAction.TYPE:
                action = this.readFromBuffer_move(buff);
                break;
            case StopMovementAction_1.StopMovementAction.TYPE:
                action = this.readFromBuffer_stopMovement(buff);
                break;
            case TurnAction_1.TurnAction.TYPE:
                action = this.readFromBuffer_turn(buff);
                break;
            case ChopTreeAction_1.ChopTreeAction.TYPE:
                action = this.readFromBuffer_chopTree(buff);
                break;
            case BuildNexusByUnitAction_1.BuildNexusByUnitAction.TYPE:
                action = this.readFromBuffer_buildNexusByUnit(buff);
                break;
            case JoinNexusTeamByUnitAction_1.JoinNexusTeamByUnitAction.TYPE:
                action = this.readFromBuffer_joinNexusTeamByUnit(buff);
                break;
            default:
                throw new LogicError_1.LogicError('Logic error.');
        }
        return action;
    }
    static readFromBuffer_move(buff) {
        let action = new MoveAction_1.MoveAction(MoveAction_1.MoveAction.TYPE, buff.readInt(), //entityId
        buff.readFloat(), //destX
        buff.readFloat() //destY
        );
        return action;
    }
    static readFromBuffer_stopMovement(buff) {
        let action = new StopMovementAction_1.StopMovementAction(StopMovementAction_1.StopMovementAction.TYPE, buff.readInt());
        return action;
    }
    static readFromBuffer_turn(buff) {
        let action = new TurnAction_1.TurnAction(TurnAction_1.TurnAction.TYPE, buff.readInt(), //entityId
        buff.readFloat() //rotDZ
        );
        return action;
    }
    static readFromBuffer_chopTree(buff) {
        let action = new ChopTreeAction_1.ChopTreeAction(ChopTreeAction_1.ChopTreeAction.TYPE, buff.readInt(), //entityId
        buff.readInt(), //treeEntityId
        new Pos3D_1.Pos3D(buff.readFloat(), buff.readFloat(), buff.readFloat()), //treePos
        buff.readInt(), //sortedDynamicVoxelIndex
        buff.readString() //collectedItemId
        );
        return action;
    }
    static readFromBuffer_buildNexusByUnit(buff) {
        let action = new BuildNexusByUnitAction_1.BuildNexusByUnitAction(BuildNexusByUnitAction_1.BuildNexusByUnitAction.TYPE, buff.readInt(), //entityId
        new Pos3D_1.Pos3D(buff.readFloat(), buff.readFloat(), buff.readFloat()) //buildPos
        );
        return action;
    }
    static readFromBuffer_joinNexusTeamByUnit(buff) {
        let action = new JoinNexusTeamByUnitAction_1.JoinNexusTeamByUnitAction(JoinNexusTeamByUnitAction_1.JoinNexusTeamByUnitAction.TYPE, buff.readInt(), //entityId
        buff.readInt() //targetNexusEntityId
        );
        return action;
    }
}
exports.ActionSerializer = ActionSerializer;
//# sourceMappingURL=ActionSerializer.js.map