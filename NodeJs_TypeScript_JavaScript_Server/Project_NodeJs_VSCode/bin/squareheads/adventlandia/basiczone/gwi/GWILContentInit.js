"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWILEntities_1 = require("./GWILEntities");
const EntityTemplatesLocal_1 = require("./config/EntityTemplatesLocal");
const Pos3D_1 = require("base/util/Pos3D");
const Quaternion3D_1 = require("base/util/Quaternion3D");
const Vector3_1 = require("base/util/Vector3");
const MovingEntityTemplate_1 = require("./entity/template/MovingEntityTemplate");
const WorldEntityLocal_1 = require("./entity/WorldEntityLocal");
const LivingEntityTemplate_1 = require("./entity/template/LivingEntityTemplate");
const VoxelInventoryTemplate_1 = require("./entity/template/VoxelInventoryTemplate");
const LumberjackTemplate_1 = require("./entity/template/LumberjackTemplate");
const BuilderTemplate_1 = require("./entity/template/BuilderTemplate");
const TeamTemplate_1 = require("./entity/template/TeamTemplate");
const MovingEntityModule_1 = require("./entity/module/moving/MovingEntityModule");
const LivingEntityModule_1 = require("./entity/module/living/LivingEntityModule");
const VoxelInventoryModule_1 = require("./entity/module/voxelinventory/VoxelInventoryModule");
const LumberjackModule_1 = require("./entity/module/lumberjack/LumberjackModule");
const BuilderModule_1 = require("./entity/module/builder/BuilderModule");
const TeamModule_1 = require("./entity/module/team/TeamModule");
class GWILContentInit extends GWILEntities_1.GWILEntities {
    constructor(handler, config) {
        super(handler, config);
        //void
    }
    create_content() {
        super.create_content();
        //void
    }
    onPlayerCreated(pl) {
        super.onPlayerCreated(pl);
        let islandGridDiameter = 18 * 10 + 1;
        let islandGridCenterX = (islandGridDiameter - 1) / 2;
        let islandGridCenterY = (islandGridDiameter - 1) / 2;
        let voxelSize = 0.1;
        let entity = this.createEntity(EntityTemplatesLocal_1.EntityTemplatesLocal.Unit_Player.id, null, pl, new Pos3D_1.Pos3D(islandGridCenterX * voxelSize, islandGridCenterY * voxelSize, 0), 
        // Quaternion for a Z rotation of -90 degrees (unit facing south towards the camera).
        Quaternion3D_1.Quaternion3D.fromAxisAngle(Vector3_1.Vector3.Z, -90));
    }
    onPlayerRemove_pre(pl) {
        super.onPlayerRemove_pre(pl);
        this.removeAllEntitiesWhereOwnerPlayer(pl);
    }
    initEntityModule(entity, subTemplate) {
        if (super.initEntityModule(entity, subTemplate)) {
            return true;
        }
        if (subTemplate.moduleId == MovingEntityTemplate_1.MovingEntityTemplate.MODULE_ID) {
            this.createUnit_movingEntity(entity, subTemplate);
        }
        else if (subTemplate.moduleId == LivingEntityTemplate_1.LivingEntityTemplate.MODULE_ID) {
            this.createUnit_livingEntity(entity, subTemplate);
        }
        else if (subTemplate.moduleId == VoxelInventoryTemplate_1.VoxelInventoryTemplate.MODULE_ID) {
            this.createUnit_voxelInventory(entity, subTemplate);
        }
        else if (subTemplate.moduleId == LumberjackTemplate_1.LumberjackTemplate.MODULE_ID) {
            this.createUnit_lumberjack(entity, subTemplate);
        }
        else if (subTemplate.moduleId == BuilderTemplate_1.BuilderTemplate.MODULE_ID) {
            this.createUnit_builder(entity, subTemplate);
        }
        else if (subTemplate.moduleId == TeamTemplate_1.TeamTemplate.MODULE_ID) {
            this.createUnit_team(entity, subTemplate);
        }
        else {
            return false;
        }
        return true;
    }
    createUnit_movingEntity(entity, movingEntityTemplate) {
        let movingEntity = new MovingEntityModule_1.MovingEntityModule(entity, movingEntityTemplate);
        entity.movingEntity = movingEntity;
    }
    createUnit_livingEntity(entity, livingEntityTemplate) {
        let livingEntity = new LivingEntityModule_1.LivingEntityModule(entity, livingEntityTemplate);
        entity.livingEntity = livingEntity;
    }
    createUnit_voxelInventory(entity, visualInventoryTemplate) {
        let voxelInventory = new VoxelInventoryModule_1.VoxelInventoryModule(entity, visualInventoryTemplate);
        entity.voxelInventory = voxelInventory;
    }
    createUnit_lumberjack(entity, lumberjackTemplate) {
        let lumberjack = new LumberjackModule_1.LumberjackModule(entity, lumberjackTemplate);
        entity.lumberjack = lumberjack;
    }
    createUnit_builder(entity, builderTemplate) {
        let builder = new BuilderModule_1.BuilderModule(entity, builderTemplate);
        entity.builder = builder;
    }
    createUnit_team(entity, teamTemplate) {
        let team = new TeamModule_1.TeamModule(entity, teamTemplate);
        entity.team = team;
    }
    createEntity_base(template, entityId) {
        let entity = new WorldEntityLocal_1.WorldEntityLocal(template, entityId, this, this.getGameWorldSnapshotDelta());
        return entity;
    }
}
exports.GWILContentInit = GWILContentInit;
//# sourceMappingURL=GWILContentInit.js.map