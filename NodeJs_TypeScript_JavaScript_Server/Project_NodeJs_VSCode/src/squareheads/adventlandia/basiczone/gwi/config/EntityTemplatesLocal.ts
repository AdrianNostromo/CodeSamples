import { EntityTemplatesBase } from "../common/config/EntityTemplatesBase";
import { EntityTemplate } from "localbase/gwi/entity/template/EntityTemplate";
import { EntityTagsLocal } from "./EntityTagsLocal";
import { MovingEntityTemplate } from "../entity/template/MovingEntityTemplate";
import { VoxelInventoryTemplate } from "../entity/template/VoxelInventoryTemplate";
import { LumberjackTemplate } from "../entity/template/LumberjackTemplate";
import { BuilderTemplate } from "../entity/template/BuilderTemplate";
import { TeamTemplate } from "../entity/template/TeamTemplate";
import { LivingEntityTemplate } from "../entity/template/LivingEntityTemplate";
import { BuildableTemplate } from "../entity/template/BuildableTemplate";
import { ResourceCost } from "../util/ResourceCost";

// Extend the common class to make sure it's entries are loaded.
export class EntityTemplatesLocal extends EntityTemplatesBase {

  public static Unit_Player: EntityTemplate = new EntityTemplate(
    "Based_Unit_Player",
    [
        EntityTagsLocal.BASED,
        EntityTagsLocal.UNIT
    ]
  )
  .setModuleTemplate(new MovingEntityTemplate(
    1
  ))
  .setModuleTemplate(new VoxelInventoryTemplate(
    //void
  ))
  .setModuleTemplate(new LumberjackTemplate(
    //void
  ))
  .setModuleTemplate(new BuilderTemplate(
    //void
  ))
  .setModuleTemplate(new TeamTemplate(
    //void
  ));
  
  public static Flora_Tree: EntityTemplate = new EntityTemplate(
    "Based_Flora_Tree",
    [
      EntityTagsLocal.BASED,
      EntityTagsLocal.FLORA,
      EntityTagsLocal.STRUCTURE,
      EntityTagsLocal.TREE,
      EntityTagsLocal.WOOD
    ]
  )
  .setModuleTemplate(new LivingEntityTemplate(
    184
  ));

  public static Building_Nexus: EntityTemplate = new EntityTemplate(
    "Based_Building_NexusA",
    [
      EntityTagsLocal.BASED,
      EntityTagsLocal.BUILDING,
      EntityTagsLocal.STRUCTURE,
      EntityTagsLocal.NEXUS
    ]
  )
  .setModuleTemplate(new BuildableTemplate(
    [
      new ResourceCost(
        [
          EntityTagsLocal.ITEM,
          EntityTagsLocal.WOOD
        ],
        5
      )
    ]
  ))
  .setModuleTemplate(new TeamTemplate(
    //void
  ));

  public static Item_Wood: EntityTemplate = new EntityTemplate(
    "Based_Item_Wood",
    [
      EntityTagsLocal.BASED,
      EntityTagsLocal.ITEM,
      EntityTagsLocal.WOOD
    ]
  );
  
  public static Item_GreenWood: EntityTemplate = new EntityTemplate(
    "Based_Item_GreenWood",
    [
      EntityTagsLocal.BASED,
      EntityTagsLocal.ITEM,
      EntityTagsLocal.WOOD
    ]
  );

}
