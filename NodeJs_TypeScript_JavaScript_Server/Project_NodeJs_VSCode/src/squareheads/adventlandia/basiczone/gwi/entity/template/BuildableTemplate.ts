import { BasicModuleTemplate } from "localbase/gwi/entity/template/BasicModuleTemplate";
import { ResourceCost } from "../../util/ResourceCost";

export class BuildableTemplate extends BasicModuleTemplate {
  
  public static readonly MODULE_ID: string = "buildable";
  
  public buildCostList: ResourceCost[];

  public constructor(buildCostList: ResourceCost[]) {
    super(BuildableTemplate.MODULE_ID, false);
    
    this.buildCostList = buildCostList;
  }

}
