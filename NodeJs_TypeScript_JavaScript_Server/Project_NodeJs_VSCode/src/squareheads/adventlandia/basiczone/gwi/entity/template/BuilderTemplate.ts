import { BasicModuleTemplate } from "localbase/gwi/entity/template/BasicModuleTemplate";

export class BuilderTemplate extends BasicModuleTemplate {
    
  public static readonly MODULE_ID: string = "builder";
  
  public constructor() {
    super(BuilderTemplate.MODULE_ID, true);
    
    //void
  }

}
