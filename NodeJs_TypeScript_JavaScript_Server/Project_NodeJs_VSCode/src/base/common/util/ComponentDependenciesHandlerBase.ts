import { BasicAppComponent } from "base/component/BasicAppComponent";

import { IApp } from "base/app/IApp";
import { ComponentType } from "base/common/ComponentType";
import { IpPort } from "base/net/util/IpPort";

export class ComponentDependenciesHandlerBase extends BasicAppComponent {

  // These dependencies are cleared every time they are required. Except for the server Manager.
  // .componentTypeID: {ip: string, port: int};
  private _dependencyComponentListenersIpPort: Map<number, IpPort> = new Map();

  public constructor(app: IApp) {
    super(app);
    
    //void
  }
  
  public addDependencyComponentListenerIpPort(componentType: ComponentType, ipPort: IpPort): boolean {
    if(this._dependencyComponentListenersIpPort.get(componentType.id)) {
      return false;
    }
    
    this._dependencyComponentListenersIpPort.set(componentType.id, ipPort);
    
    return true;
  }
  
  public clearDependencyComponentListenerIpPort(componentType: ComponentType): void {
    this._dependencyComponentListenersIpPort.delete(componentType.id);
  }
  
  public getDependencyComponentListenerIpPort(componentType: ComponentType): IpPort {
    let ret: IpPort = this._dependencyComponentListenersIpPort.get(componentType.id);
    
    return ret;
  }
  
}
