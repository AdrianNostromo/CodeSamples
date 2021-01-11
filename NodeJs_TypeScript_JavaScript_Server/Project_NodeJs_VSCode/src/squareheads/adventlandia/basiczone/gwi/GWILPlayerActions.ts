import { GWILForestGrower } from "./GWILForestGrower";
import { IHandler_GWI } from "localbase/gwi/IHandler_GWI";
import { DTGWIConfigBasicZone } from "./util/DTGWIConfigBasicZone";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { BasicAction } from "localbase/gwi/action/BasicAction";
import { ActionSerializer } from "./action/ActionSerializer";
import { BasicPlayer } from "localbase/gwi/util/BasicPlayer";
import { MoveAction } from "./action/MoveAction";
import { StopMovementAction } from "./action/StopMovementAction";
import { TurnAction } from "./action/TurnAction";
import { ChopTreeAction } from "./action/ChopTreeAction";
import { BuildNexusByUnitAction } from "./action/BuildNexusByUnitAction";
import { JoinNexusTeamByUnitAction } from "./action/JoinNexusTeamByUnitAction";
import { LogicError } from "base/exception/LogicError";
import { WorldEntityLocal } from "./entity/WorldEntityLocal";
import { SecurityError } from "base/exception/SecurityError";
import { EntityTemplate } from "localbase/gwi/entity/template/EntityTemplate";
import { EntityTemplatesLocal } from "./config/EntityTemplatesLocal";

export abstract class GWILPlayerActions extends GWILForestGrower {

  public constructor(handler: IHandler_GWI, config: DTGWIConfigBasicZone) {
    super(handler, config);
    
    //void
  }
  
  protected processMsgPart_readPlayerActionEntry(msgNetBuffer: StreamBuffer): BasicAction {
    let action: BasicAction
    
    try {
      action = ActionSerializer.readFromBuffer(msgNetBuffer);
    }catch(x) {
      throw x;
    }
    
    return action;
  }

  protected processPlayerAction(pl: BasicPlayer, action: BasicAction): void {
    switch (action.type) {
      case MoveAction.TYPE:
        this.processPlayerAction_moveAction(pl, <MoveAction>action);
        
        break;
      case StopMovementAction.TYPE:
        this.processPlayerAction_stopMovement(pl, <StopMovementAction>action);

        break;
      case TurnAction.TYPE:
        this.processPlayerAction_turn(pl, <TurnAction>action);

        break;
        case ChopTreeAction.TYPE:
          this.processPlayerAction_chopTree(pl, <ChopTreeAction>action);
  
        break;
        case BuildNexusByUnitAction.TYPE:
          this.processPlayerAction_buildNexusByUnit(pl, <BuildNexusByUnitAction>action);
  
        break;
        case JoinNexusTeamByUnitAction.TYPE:
          this.processPlayerAction_joinNexusTeamByUnit(pl, <JoinNexusTeamByUnitAction>action);
  
          break;
      default:
        throw new LogicError('Logic error.');
    }
  }
  
  protected processPlayerAction_moveAction(pl: BasicPlayer, action: MoveAction): void {
    let entity: WorldEntityLocal = this.getEntityWithEntityId<WorldEntityLocal>(action.entityId);
    if(entity == null || entity.isMarkedForRemoval) {
      throw new SecurityError("Player tried to execute an action on a missing entity.");
    }

    // Check if the entity is owned by the player.
    if(entity.getOwnerPlayer() != pl) {
      throw new SecurityError("Player tried to execute an action on a entity that he is not the owner of.");
    }

    entity.movingEntity.move(action.destX, action.destY);
  }
  
  protected processPlayerAction_stopMovement(pl: BasicPlayer, action: StopMovementAction): void {
    let entity: WorldEntityLocal = this.getEntityWithEntityId<WorldEntityLocal>(action.entityId);
    if(entity == null || entity.isMarkedForRemoval) {
      throw new SecurityError("Player tried to execute an action on a missing entity.");
    }

    // Check if the entity is owned by the player.
    if(entity.getOwnerPlayer() != pl) {
      throw new SecurityError("Player tried to execute an action on a entity that he is not the owner of.");
    }
    
    entity.movingEntity.stopMovement();
  }
  
  protected processPlayerAction_turn(pl: BasicPlayer, action: TurnAction): void {
    let entity: WorldEntityLocal = this.getEntityWithEntityId<WorldEntityLocal>(action.entityId);
    if(entity == null) {
      // Dispose the player. Possible security warning.
      throw new SecurityError("Error.1.");
    }

    // Check if the entity is owned by the player.
    if(entity.getOwnerPlayer() != pl) {
      // Dispose the player. Possible security warning.
      throw new SecurityError("Error.1.");
    }
    
    entity.movingEntity.turn(action.rotDZ);
  }
  
  protected processPlayerAction_chopTree(pl: BasicPlayer, action: ChopTreeAction): void {
    let entity: WorldEntityLocal = this.getEntityWithEntityId<WorldEntityLocal>(action.entityId);
    if(entity == null) {
      // Dispose the player. Possible security warning.
      throw new SecurityError("Error.1.");
    }

    // Check if the entity is owned by the player.
    if(entity.getOwnerPlayer() != pl) {
      // Dispose the player. Possible security warning.
      throw new SecurityError("Error.1.");
    }

    if(entity.lumberjack == null) {
      // Dispose the player. Possible security warning.
      throw new SecurityError("Error.1.");
    }

    let collectedItemTemplate: EntityTemplate = null;
    if(action.collectedItemId == EntityTemplatesLocal.Item_Wood.id) {
      collectedItemTemplate = EntityTemplatesLocal.Item_Wood;
    }else if(action.collectedItemId == EntityTemplatesLocal.Item_GreenWood.id) {
      collectedItemTemplate = EntityTemplatesLocal.Item_GreenWood;
    }else {
      throw new SecurityError("Error.1.");
    }

    entity.lumberjack.throwChopAxe(
      action.treeEntityId,
      action.treePos,
      action.sortedDynamicVoxelIndex,
      collectedItemTemplate
    );
  }
  
  protected processPlayerAction_buildNexusByUnit(pl: BasicPlayer, action: BuildNexusByUnitAction): void {
    let entity: WorldEntityLocal = this.getEntityWithEntityId<WorldEntityLocal>(action.entityId);
    if(entity == null) {
      // Dispose the player. Possible security warning.
      throw new SecurityError("Error.1.");
    }

    // Check if the entity is owned by the player.
    if(entity.getOwnerPlayer() != pl) {
      // Dispose the player. Possible security warning.
      throw new SecurityError("Error.1.");
    }

    if(entity.builder.isRequestInProgress_buildNexusAndJoinTeam()) {
      // There is a request waiting to be processed. Duplicates should not be received.
      // Dispose the player. Possible security warning.
      throw new SecurityError("Error.1.");
    }

    entity.builder.buildNexusAndJoinTeam(
      action.buildPos
    );
  }
  
  protected processPlayerAction_joinNexusTeamByUnit(pl: BasicPlayer, action: JoinNexusTeamByUnitAction): void {
    let entity: WorldEntityLocal = this.getEntityWithEntityId<WorldEntityLocal>(action.entityId);
    if(entity == null) {
      // Dispose the player. Possible security warning.
      throw new SecurityError("Error.1.");
    }

    // Check if the entity is owned by the player.
    if(entity.getOwnerPlayer() != pl) {
      // Dispose the player. Possible security warning.
      throw new SecurityError("Error.1.");
    }

    if(entity.team.isRequestInProgress_joinNexusTeam()) {
      // There is a request waiting to be processed. Duplicates should not be received.
      // Dispose the player. Possible security warning.
      throw new SecurityError("Error.1.");
    }

    entity.team.joinNexusTeam(
      action.targetNexusEntityId
    );
  }
  
}
