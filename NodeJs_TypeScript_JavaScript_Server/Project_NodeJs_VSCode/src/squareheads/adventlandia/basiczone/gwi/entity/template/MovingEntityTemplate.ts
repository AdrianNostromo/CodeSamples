import { BasicModuleTemplate } from "localbase/gwi/entity/template/BasicModuleTemplate";

export class MovingEntityTemplate extends BasicModuleTemplate {
    
  public static readonly MODULE_ID: string = "movingEntity";
  
  public moveSpeedS: number;

  public constructor(moveSpeedS: number) {
    super(MovingEntityTemplate.MODULE_ID, true);

    this.moveSpeedS = moveSpeedS;
  }

}
