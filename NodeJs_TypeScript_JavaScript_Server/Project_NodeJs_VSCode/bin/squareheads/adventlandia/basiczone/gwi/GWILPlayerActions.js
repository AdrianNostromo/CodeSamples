"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWILForestGrower_1 = require("./GWILForestGrower");
const ActionSerializer_1 = require("./action/ActionSerializer");
const MoveAction_1 = require("./action/MoveAction");
const StopMovementAction_1 = require("./action/StopMovementAction");
const TurnAction_1 = require("./action/TurnAction");
const ChopTreeAction_1 = require("./action/ChopTreeAction");
const BuildNexusByUnitAction_1 = require("./action/BuildNexusByUnitAction");
const JoinNexusTeamByUnitAction_1 = require("./action/JoinNexusTeamByUnitAction");
const LogicError_1 = require("base/exception/LogicError");
const SecurityError_1 = require("base/exception/SecurityError");
const EntityTemplatesLocal_1 = require("./config/EntityTemplatesLocal");
class GWILPlayerActions extends GWILForestGrower_1.GWILForestGrower {
    constructor(handler, config) {
        super(handler, config);
        //void
    }
    processMsgPart_readPlayerActionEntry(msgNetBuffer) {
        let action;
        try {
            action = ActionSerializer_1.ActionSerializer.readFromBuffer(msgNetBuffer);
        }
        catch (x) {
            throw x;
        }
        return action;
    }
    processPlayerAction(pl, action) {
        switch (action.type) {
            case MoveAction_1.MoveAction.TYPE:
                this.processPlayerAction_moveAction(pl, action);
                break;
            case StopMovementAction_1.StopMovementAction.TYPE:
                this.processPlayerAction_stopMovement(pl, action);
                break;
            case TurnAction_1.TurnAction.TYPE:
                this.processPlayerAction_turn(pl, action);
                break;
            case ChopTreeAction_1.ChopTreeAction.TYPE:
                this.processPlayerAction_chopTree(pl, action);
                break;
            case BuildNexusByUnitAction_1.BuildNexusByUnitAction.TYPE:
                this.processPlayerAction_buildNexusByUnit(pl, action);
                break;
            case JoinNexusTeamByUnitAction_1.JoinNexusTeamByUnitAction.TYPE:
                this.processPlayerAction_joinNexusTeamByUnit(pl, action);
                break;
            default:
                throw new LogicError_1.LogicError('Logic error.');
        }
    }
    processPlayerAction_moveAction(pl, action) {
        let entity = this.getEntityWithEntityId(action.entityId);
        if (entity == null || entity.isMarkedForRemoval) {
            throw new SecurityError_1.SecurityError("Player tried to execute an action on a missing entity.");
        }
        // Check if the entity is owned by the player.
        if (entity.getOwnerPlayer() != pl) {
            throw new SecurityError_1.SecurityError("Player tried to execute an action on a entity that he is not the owner of.");
        }
        entity.movingEntity.move(action.destX, action.destY);
    }
    processPlayerAction_stopMovement(pl, action) {
        let entity = this.getEntityWithEntityId(action.entityId);
        if (entity == null || entity.isMarkedForRemoval) {
            throw new SecurityError_1.SecurityError("Player tried to execute an action on a missing entity.");
        }
        // Check if the entity is owned by the player.
        if (entity.getOwnerPlayer() != pl) {
            throw new SecurityError_1.SecurityError("Player tried to execute an action on a entity that he is not the owner of.");
        }
        entity.movingEntity.stopMovement();
    }
    processPlayerAction_turn(pl, action) {
        let entity = this.getEntityWithEntityId(action.entityId);
        if (entity == null) {
            // Dispose the player. Possible security warning.
            throw new SecurityError_1.SecurityError("Error.1.");
        }
        // Check if the entity is owned by the player.
        if (entity.getOwnerPlayer() != pl) {
            // Dispose the player. Possible security warning.
            throw new SecurityError_1.SecurityError("Error.1.");
        }
        entity.movingEntity.turn(action.rotDZ);
    }
    processPlayerAction_chopTree(pl, action) {
        let entity = this.getEntityWithEntityId(action.entityId);
        if (entity == null) {
            // Dispose the player. Possible security warning.
            throw new SecurityError_1.SecurityError("Error.1.");
        }
        // Check if the entity is owned by the player.
        if (entity.getOwnerPlayer() != pl) {
            // Dispose the player. Possible security warning.
            throw new SecurityError_1.SecurityError("Error.1.");
        }
        if (entity.lumberjack == null) {
            // Dispose the player. Possible security warning.
            throw new SecurityError_1.SecurityError("Error.1.");
        }
        let collectedItemTemplate = null;
        if (action.collectedItemId == EntityTemplatesLocal_1.EntityTemplatesLocal.Item_Wood.id) {
            collectedItemTemplate = EntityTemplatesLocal_1.EntityTemplatesLocal.Item_Wood;
        }
        else if (action.collectedItemId == EntityTemplatesLocal_1.EntityTemplatesLocal.Item_GreenWood.id) {
            collectedItemTemplate = EntityTemplatesLocal_1.EntityTemplatesLocal.Item_GreenWood;
        }
        else {
            throw new SecurityError_1.SecurityError("Error.1.");
        }
        entity.lumberjack.throwChopAxe(action.treeEntityId, action.treePos, action.sortedDynamicVoxelIndex, collectedItemTemplate);
    }
    processPlayerAction_buildNexusByUnit(pl, action) {
        let entity = this.getEntityWithEntityId(action.entityId);
        if (entity == null) {
            // Dispose the player. Possible security warning.
            throw new SecurityError_1.SecurityError("Error.1.");
        }
        // Check if the entity is owned by the player.
        if (entity.getOwnerPlayer() != pl) {
            // Dispose the player. Possible security warning.
            throw new SecurityError_1.SecurityError("Error.1.");
        }
        if (entity.builder.isRequestInProgress_buildNexusAndJoinTeam()) {
            // There is a request waiting to be processed. Duplicates should not be received.
            // Dispose the player. Possible security warning.
            throw new SecurityError_1.SecurityError("Error.1.");
        }
        entity.builder.buildNexusAndJoinTeam(action.buildPos);
    }
    processPlayerAction_joinNexusTeamByUnit(pl, action) {
        let entity = this.getEntityWithEntityId(action.entityId);
        if (entity == null) {
            // Dispose the player. Possible security warning.
            throw new SecurityError_1.SecurityError("Error.1.");
        }
        // Check if the entity is owned by the player.
        if (entity.getOwnerPlayer() != pl) {
            // Dispose the player. Possible security warning.
            throw new SecurityError_1.SecurityError("Error.1.");
        }
        if (entity.team.isRequestInProgress_joinNexusTeam()) {
            // There is a request waiting to be processed. Duplicates should not be received.
            // Dispose the player. Possible security warning.
            throw new SecurityError_1.SecurityError("Error.1.");
        }
        entity.team.joinNexusTeam(action.targetNexusEntityId);
    }
}
exports.GWILPlayerActions = GWILPlayerActions;
//# sourceMappingURL=GWILPlayerActions.js.map