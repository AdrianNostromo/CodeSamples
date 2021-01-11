"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const EntityTemplatesBase_1 = require("../common/config/EntityTemplatesBase");
const EntityTemplate_1 = require("localbase/gwi/entity/template/EntityTemplate");
const EntityTagsLocal_1 = require("./EntityTagsLocal");
const MovingEntityTemplate_1 = require("../entity/template/MovingEntityTemplate");
const VoxelInventoryTemplate_1 = require("../entity/template/VoxelInventoryTemplate");
const LumberjackTemplate_1 = require("../entity/template/LumberjackTemplate");
const BuilderTemplate_1 = require("../entity/template/BuilderTemplate");
const TeamTemplate_1 = require("../entity/template/TeamTemplate");
const LivingEntityTemplate_1 = require("../entity/template/LivingEntityTemplate");
const BuildableTemplate_1 = require("../entity/template/BuildableTemplate");
const ResourceCost_1 = require("../util/ResourceCost");
// Extend the common class to make sure it's entries are loaded.
class EntityTemplatesLocal extends EntityTemplatesBase_1.EntityTemplatesBase {
}
EntityTemplatesLocal.Unit_Player = new EntityTemplate_1.EntityTemplate("Based_Unit_Player", [
    EntityTagsLocal_1.EntityTagsLocal.BASED,
    EntityTagsLocal_1.EntityTagsLocal.UNIT
])
    .setModuleTemplate(new MovingEntityTemplate_1.MovingEntityTemplate(1))
    .setModuleTemplate(new VoxelInventoryTemplate_1.VoxelInventoryTemplate(
//void
))
    .setModuleTemplate(new LumberjackTemplate_1.LumberjackTemplate(
//void
))
    .setModuleTemplate(new BuilderTemplate_1.BuilderTemplate(
//void
))
    .setModuleTemplate(new TeamTemplate_1.TeamTemplate(
//void
));
EntityTemplatesLocal.Flora_Tree = new EntityTemplate_1.EntityTemplate("Based_Flora_Tree", [
    EntityTagsLocal_1.EntityTagsLocal.BASED,
    EntityTagsLocal_1.EntityTagsLocal.FLORA,
    EntityTagsLocal_1.EntityTagsLocal.STRUCTURE,
    EntityTagsLocal_1.EntityTagsLocal.TREE,
    EntityTagsLocal_1.EntityTagsLocal.WOOD
])
    .setModuleTemplate(new LivingEntityTemplate_1.LivingEntityTemplate(184));
EntityTemplatesLocal.Building_Nexus = new EntityTemplate_1.EntityTemplate("Based_Building_NexusA", [
    EntityTagsLocal_1.EntityTagsLocal.BASED,
    EntityTagsLocal_1.EntityTagsLocal.BUILDING,
    EntityTagsLocal_1.EntityTagsLocal.STRUCTURE,
    EntityTagsLocal_1.EntityTagsLocal.NEXUS
])
    .setModuleTemplate(new BuildableTemplate_1.BuildableTemplate([
    new ResourceCost_1.ResourceCost([
        EntityTagsLocal_1.EntityTagsLocal.ITEM,
        EntityTagsLocal_1.EntityTagsLocal.WOOD
    ], 5)
]))
    .setModuleTemplate(new TeamTemplate_1.TeamTemplate(
//void
));
EntityTemplatesLocal.Item_Wood = new EntityTemplate_1.EntityTemplate("Based_Item_Wood", [
    EntityTagsLocal_1.EntityTagsLocal.BASED,
    EntityTagsLocal_1.EntityTagsLocal.ITEM,
    EntityTagsLocal_1.EntityTagsLocal.WOOD
]);
EntityTemplatesLocal.Item_GreenWood = new EntityTemplate_1.EntityTemplate("Based_Item_GreenWood", [
    EntityTagsLocal_1.EntityTagsLocal.BASED,
    EntityTagsLocal_1.EntityTagsLocal.ITEM,
    EntityTagsLocal_1.EntityTagsLocal.WOOD
]);
exports.EntityTemplatesLocal = EntityTemplatesLocal;
//# sourceMappingURL=EntityTemplatesLocal.js.map