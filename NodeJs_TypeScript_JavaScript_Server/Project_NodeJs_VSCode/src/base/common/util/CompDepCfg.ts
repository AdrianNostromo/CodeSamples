import { ComponentType } from "base/common/ComponentType";
import { ComponentStatus } from "base/common/util/ComponentStatus";

export class CompDepCfg {

  public componentType: ComponentType;
  
  public componentStatusLayer: ComponentStatus;

  constructor(componentType: ComponentType, componentStatusLayer: ComponentStatus) {
    this.componentType = componentType;
    
    this.componentStatusLayer = componentStatusLayer;
  }
  
}
