import { BasicAction } from "localbase/gwi/action/BasicAction";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { MoveAction } from "./MoveAction";
import { StopMovementAction } from "./StopMovementAction";
import { TurnAction } from "./TurnAction";
import { ChopTreeAction } from "./ChopTreeAction";
import { BuildNexusByUnitAction } from "./BuildNexusByUnitAction";
import { JoinNexusTeamByUnitAction } from "./JoinNexusTeamByUnitAction";
import { LogicError } from "base/exception/LogicError";
import { Pos3D } from "base/util/Pos3D";

export class ActionSerializer {
  
  /**
   * 
   * @param buff 
   * @throws Error
   */
  public static readFromBuffer(buff: StreamBuffer): BasicAction {
    let type: number = buff.readInt();

    let action: BasicAction = null;

    switch (type) {
      case MoveAction.TYPE:
        action = this.readFromBuffer_move(buff);

        break;
      case StopMovementAction.TYPE:
        action = this.readFromBuffer_stopMovement(buff);
        
        break;
      case TurnAction.TYPE:
        action = this.readFromBuffer_turn(buff);

        break;
      case ChopTreeAction.TYPE:
        action = this.readFromBuffer_chopTree(buff);

        break;
      case BuildNexusByUnitAction.TYPE:
        action = this.readFromBuffer_buildNexusByUnit(buff);

        break;
      case JoinNexusTeamByUnitAction.TYPE:
        action = this.readFromBuffer_joinNexusTeamByUnit(buff);

        break;
      default:
        throw new LogicError('Logic error.');
    }

    return action;
  }

  public static readFromBuffer_move(buff: StreamBuffer): BasicAction {
    let action: BasicAction = new MoveAction(
      MoveAction.TYPE,
      buff.readInt(),//entityId
      buff.readFloat(),//destX
      buff.readFloat()//destY
    );

    return action;
  }

  public static readFromBuffer_stopMovement(buff: StreamBuffer): BasicAction {
    let action: BasicAction = new StopMovementAction(
      StopMovementAction.TYPE,
      buff.readInt(),//entityId
    );

    return action;
  }

  public static readFromBuffer_turn(buff: StreamBuffer): BasicAction {
    let action: BasicAction = new TurnAction(
      TurnAction.TYPE,
      buff.readInt(),//entityId
      buff.readFloat()//rotDZ
    );

    return action;
  }

  public static readFromBuffer_chopTree(buff: StreamBuffer): BasicAction {
    let action: BasicAction = new ChopTreeAction(
      ChopTreeAction.TYPE,
      buff.readInt(),//entityId
      buff.readInt(),//treeEntityId
      new Pos3D(
        buff.readFloat(),
        buff.readFloat(),
        buff.readFloat()
      ),//treePos
      buff.readInt(),//sortedDynamicVoxelIndex
      buff.readString()//collectedItemId
    );

    return action;
  }

  public static readFromBuffer_buildNexusByUnit(buff: StreamBuffer): BasicAction {
    let action: BasicAction = new BuildNexusByUnitAction(
      BuildNexusByUnitAction.TYPE,
      buff.readInt(),//entityId
      new Pos3D(
        buff.readFloat(),
        buff.readFloat(),
        buff.readFloat()
      )//buildPos
    );

    return action;
  }

  public static readFromBuffer_joinNexusTeamByUnit(buff: StreamBuffer): BasicAction {
    let action: BasicAction = new JoinNexusTeamByUnitAction(
      JoinNexusTeamByUnitAction.TYPE,
      buff.readInt(),//entityId
      buff.readInt()//targetNexusEntityId
    );

    return action;
  }

}
  