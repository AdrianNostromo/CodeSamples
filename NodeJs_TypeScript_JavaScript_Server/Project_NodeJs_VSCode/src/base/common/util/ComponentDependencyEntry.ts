import { ComponentType } from "./../ComponentType";
import { ComponentStatus } from "./../util/ComponentStatus";

export class ComponentDependencyEntry {

  public componentType: ComponentType;
  public componentStatusLayer: ComponentStatus;

  public constructor(componentType: ComponentType, componentStatusLayer: ComponentStatus) {
    this.componentType = componentType;
    this.componentStatusLayer = componentStatusLayer
  }

}