import { BasicModuleTemplate } from "localbase/gwi/entity/template/BasicModuleTemplate";

export class TeamTemplate extends BasicModuleTemplate {
    
  public static readonly MODULE_ID: string = "team";
  
  public constructor() {
    super(TeamTemplate.MODULE_ID, true);
    
    //void
  }

}
