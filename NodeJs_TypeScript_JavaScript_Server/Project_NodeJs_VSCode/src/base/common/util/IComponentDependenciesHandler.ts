import { ComponentType } from "../ComponentType";
import { IpPort } from "base/net/util/IpPort";
import { ComponentStatus } from "./ComponentStatus";

export interface IComponentDependenciesHandler {

    addDependencyComponentListenerIpPort(componentType: ComponentType, ipPort: IpPort): boolean;
    addDependency(componentType: ComponentType, componentStatusLayer: ComponentStatus): boolean;
    
}
