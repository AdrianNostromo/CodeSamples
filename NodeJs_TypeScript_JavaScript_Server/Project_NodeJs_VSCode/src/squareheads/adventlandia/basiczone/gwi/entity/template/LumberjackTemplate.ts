import { BasicModuleTemplate } from "localbase/gwi/entity/template/BasicModuleTemplate";

export class LumberjackTemplate extends BasicModuleTemplate {
    
  public static readonly MODULE_ID: string = "lumberjack";
  
  public constructor() {
    super(LumberjackTemplate.MODULE_ID, true);
    
    //void
  }

}
