"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWIGrid_1 = require("./GWIGrid");
const LogicError_1 = require("base/exception/LogicError");
const EntityTemplate_1 = require("./entity/template/EntityTemplate");
const EntityRemoveReason_1 = require("./entity/util/EntityRemoveReason");
class GWIEntities extends GWIGrid_1.GWIGrid {
    constructor(handler, config) {
        super(handler, config);
        this.entitiesList = [];
        this.entitiesMap = new Map();
        this.entitiesList_withGameLoop = [];
        this.markedForRemovalEntities = [];
        this.entityIdIncrementor = 0;
        //void
    }
    getEntitiesList() {
        return this.entitiesList;
    }
    gameLoop_simulation(deltaS, deltaMS) {
        super.gameLoop_simulation(deltaS, deltaMS);
        for (let i = 0; i < this.entitiesList_withGameLoop.length; i++) {
            let entity = this.entitiesList_withGameLoop[i];
            for (let j = 0; j < entity.modulesList_withGameLoop.length; j++) {
                let mod = entity.modulesList_withGameLoop[j];
                mod.gameLoop(deltaS, deltaMS);
            }
        }
    }
    gameLoop_entitiesRemoval(deltaS, deltaMS) {
        super.gameLoop_entitiesRemoval(deltaS, deltaMS);
        if (this.markedForRemovalEntities.length > 0) {
            for (let i = this.markedForRemovalEntities.length - 1; i >= 0; i--) {
                let entity = this.markedForRemovalEntities[i];
                if (!entity.isMarkedForRemoval) {
                    throw new LogicError_1.LogicError("GameWorldEntities.createEntity.1.");
                }
                this.removeEntity(entity);
            }
            this.markedForRemovalEntities = [];
        }
    }
    /**
     *
     * @param template
     * @param pos
     * @param rot
     * @param instanceId
     * @param isPlayerControllable
     */
    createEntity(templateId, instanceId, ownerPlayer, pos, rot) {
        let template = EntityTemplate_1.EntityTemplate.entriesMap.get(templateId);
        if (template == null) {
            throw new LogicError_1.LogicError("GameWorldEntities.createEntity.1.");
        }
        let entityId = ++this.entityIdIncrementor;
        let entity = this.createEntityObject(template, entityId);
        if (ownerPlayer != null) {
            entity.setOwnerPlayer(ownerPlayer);
        }
        entity.setPos(pos);
        entity.setRot(rot);
        if (instanceId != null) {
            entity.setInstanceId(instanceId);
            // Save the entity in the entities map.
            this.entitiesMap.set(instanceId, entity);
        }
        this.entitiesList.push(entity);
        if (entity.modulesList_withGameLoop.length > 0) {
            this.entitiesList_withGameLoop.push(entity);
        }
        this.onEntityCreated(entity);
        return entity;
    }
    onPlayerRemove_pre(pl) {
        super.onPlayerRemove_pre(pl);
        //asdz;// remove all the player entities when he is removed.
    }
    createEntityObject(template, entityId) {
        let entity = this.createEntity_base(template, entityId);
        if (entity == null) {
            throw new LogicError_1.LogicError("GameWorldEntities.createEntity.2.");
        }
        for (let i = 0; i < entity.template.moduleTemplatesList.length; i++) {
            let subTemplate = entity.template.moduleTemplatesList[i];
            if (subTemplate.hasModule && !this.initEntityModule(entity, subTemplate)) {
                throw new LogicError_1.LogicError("GameWorldEntitiesCreator.createUnit_visual3D.1.");
            }
        }
        return entity;
    }
    initEntityModule(entity, subTemplate) {
        //void
        return false;
    }
    // Extendable.
    onEntityCreated(entity) {
        //void
    }
    // Extendable.
    onEntityRemoved_pre(entity) {
        //void
    }
    markEntityForRemoval(entity, removeReason) {
        entity.isMarkedForRemoval = true;
        this.markedForRemovalEntities.push(entity);
    }
    removeEntity(entity) {
        let index = this.entitiesList.indexOf(entity);
        if (index < 0) {
            throw new LogicError_1.LogicError("GameWorldBase.upgradePreInitMatch.1.");
        }
        entity.onEntityRemove_pre();
        this.onEntityRemoved_pre(entity);
        let j = this.entitiesList_withGameLoop.indexOf(entity);
        if (j >= 0) {
            this.entitiesList_withGameLoop.splice(j, 1);
        }
        if (entity.getInstanceId() != null) {
            // Remove the entity from the entities map.
            if (this.entitiesMap.get(entity.getInstanceId()) == null) {
                throw new LogicError_1.LogicError("GameWorldEntities.removeEntity.1.");
            }
            this.entitiesMap.delete(entity.getInstanceId());
            entity.setInstanceId(null);
        }
        this.entitiesList.splice(index, 1);
    }
    getFirstNotRemovingAnyEntityWhereOwnerPlayer(pl) {
        for (let i = 0; i < this.entitiesList.length; i++) {
            let entity = this.entitiesList[i];
            if (entity.getOwnerPlayer() == pl && !entity.isMarkedForRemoval) {
                return entity;
            }
        }
        return null;
    }
    getEntityWithEntityId(entityId) {
        for (let i = 0; i < this.entitiesList.length; i++) {
            let entity = this.entitiesList[i];
            if (entity != null && entity.entityId == entityId) {
                return entity;
            }
        }
        return null;
    }
    removeAllEntitiesWhereOwnerPlayer(pl) {
        let ct = 0;
        while (true) {
            let entity = this.getFirstNotRemovingAnyEntityWhereOwnerPlayer(pl);
            if (entity == null) {
                break;
            }
            this.markEntityForRemoval(entity, EntityRemoveReason_1.EntityRemoveReason.CODE_DISPOSED);
            ct++;
        }
        return ct;
    }
    dispose() {
        if (this.entitiesList != null) {
            for (let i = this.entitiesList.length - 1; i >= 0; i--) {
                let entity = this.entitiesList[i];
                if (entity != null) {
                    this.removeEntity(entity);
                }
            }
            this.entitiesList = null;
        }
        super.dispose();
    }
}
exports.GWIEntities = GWIEntities;
//# sourceMappingURL=GWIEntities.js.map