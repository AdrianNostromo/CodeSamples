import { BasicModuleTemplate } from "localbase/gwi/entity/template/BasicModuleTemplate";

export class LivingEntityTemplate extends BasicModuleTemplate {
  
  public static readonly MODULE_ID: string = "livingEntity";

  public maxHP: number;

  public constructor(maxHP: number) {
    super(LivingEntityTemplate.MODULE_ID, true);
    
    this.maxHP = maxHP;
  }

}
