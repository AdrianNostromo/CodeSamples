import { EntityTagsBase } from "../common/config/EntityTagsBase";

export class EntityTagsLocal extends EntityTagsBase {

  public static readonly BASED: string = "BASED";
  
  public static readonly BUILDING: string = "BUILDING";
  public static readonly TREE: string = "TREE";
  public static readonly ITEM: string = "ITEM";
  public static readonly UNIT: string = "UNIT";
  // Buildings, Trees, teleporter_pads, ...;
  public static readonly STRUCTURE: string = "STRUCTURE";
  public static readonly FLORA: string = "FLORA";
  public static readonly WOOD: string = "WOOD";
  public static readonly NEXUS: string = "NEXUS";
  
}
