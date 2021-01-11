import { AppServerNetcutsConnector } from "./AppServerNetcutsConnector";

import { ComponentType } from "base/common/ComponentType";
import { AppDirs } from "base/common/util/AppDirs";
import { BasicNetcutsFactory } from "base/net/BasicNetcutsFactory";

import { AppConfig } from "base/common/config/AppConfig";
import { ComponentStatus } from "base/common/util/ComponentStatus";
import { ClientListenerNetConfig } from "base/module/util/ClientListenerNetConfig";
import { ServerConnectorNetConfig } from "base/module/util/ServerConnectorNetConfig";
import { IApp } from "./IApp";

export abstract class App extends AppServerNetcutsConnector implements IApp {

  public constructor(componentType: ComponentType, dirs: AppDirs, netcutsFactory: BasicNetcutsFactory, clientListenerNetConfig: ClientListenerNetConfig, serverConnectorNetConfig: ServerConnectorNetConfig) {
    super(componentType, dirs, netcutsFactory, clientListenerNetConfig, serverConnectorNetConfig);
    
    //void
  }

  protected create_framework(): void {
    super.create_framework();
    
    this.getLog().console('Component type: ' + this._componentType.name);
  }
  
  public startApp(): void {
    if(this.getComponentStatus() != ComponentStatus.OFFLINE) {
      this.getApp().getLog().error('Exit. Initialize error 1.');

      this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
      
      return ;
    }
    
    this.setComponentStatus(ComponentStatus.STARTING);
  }
  
  public componentStatusChanged(componentStatus: ComponentStatus): void {
    super.componentStatusChanged(componentStatus);
    
    if(componentStatus == ComponentStatus.STARTING) {
      if(this._componentType != ComponentType.SERVER_MANAGER) {
        if(!this.getApp().getComponentDependenciesHandler().addDependencyComponentListenerIpPort(ComponentType.SERVER_MANAGER, AppConfig.DEFAULT_SERVER_MANAGER_IP_PORT)) {
          this.getApp().getLog().error('Exit. Initialize error 1.');

          this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
          
          return ;
        }
        
        if(!this.getApp().getComponentDependenciesHandler().addDependency(ComponentType.SERVER_MANAGER, ComponentStatus.CONNECTING)) {
          this.getApp().getLog().error('Exit. Initialize error 1.');

          this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
          
          return ;
        }
      }
      
      if(!this.getApp().getComponentDependenciesHandler().addDependency(ComponentType.LOG_MANAGER, ComponentStatus.ONLINE_FOR_COMPONENTS)) {
        this.getApp().getLog().error('Exit. Initialize error 1.');

        this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
        
        return ;
      }
    }
  }
}
