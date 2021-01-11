import { LocalAppComponent } from "./../component/LocalAppComponent";

import { IAppLocal } from "./../app/IAppLocal";

import { AppConfig } from "base/common/config/AppConfig";
import { ComponentStatus } from "base/common/util/ComponentStatus";
import { ComponentType } from "base/common/ComponentType";
import { ServerStatus } from "base/common/server/ServerStatus";
import { Netcut_SC_ServerManager_To_NodeController } from "servermanager/netcut/Netcut_SC_ServerManager_To_NodeController";
import { ILoopUserBase_General } from "base/component/loop/list/ILoopUserBase_General";

class ServerComponentConfig {
  
  public componentType: ComponentType;
  
  public count: number;
  public isAutoCreated: boolean;

  public constructor(componentType: ComponentType, count: number, isAutoCreated: boolean) {
    this.componentType = componentType;
    
    this.count = count;
    this.isAutoCreated = isAutoCreated;
  }

}

class CurrentlyCreatingComponent {

  public componentType: ComponentType;
  public curentOnlineComponentsCount: number;
  public currentWaitDurationS: number = 0;

  public constructor(componentType: ComponentType, curentOnlineComponentsCount: number) {
    this.componentType = componentType;
    this.curentOnlineComponentsCount = curentOnlineComponentsCount;
  }

}

export class ServerStructureHandler extends LocalAppComponent implements ILoopUserBase_General {
  
  public _ILoopUserBase_General: true = true;
  
  private static readonly targetServerComponentsStructure = [
    new ServerComponentConfig(ComponentType.LOG_MANAGER, 1, true),
    //new ServerComponentConfig(ComponentType.STAR_MANAGER, 1, false)//asd_tst_001;
    new ServerComponentConfig(ComponentType.SQUARE_HEADS_MANAGER, 1, false)//asd_tst_001;
  ];

  private _currentlyCreatingComponent: CurrentlyCreatingComponent = null;
  
  private _isInitialNodeControllersConnected: boolean = false;
  
  private _uLogID_A: number;
  private _uLogID_B: number;

  public constructor(app: IAppLocal) {
    super(app);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    this._uLogID_A = this.getApp().getLog().getNextULogId();
    this._uLogID_B = this.getApp().getLog().getNextULogId();
  }
  
  public appLoop_General(deltaS: number, deltaMS: number): void {
    this.appLoop_local(deltaS, deltaMS);
  }
  
  private appLoop_local(deltaS: number, deltaMS: number): void {
    if(!this.getApp().getComponentStatus().isOnline) {
      return ;
    }
    
    if(!this._isInitialNodeControllersConnected) {
      this.getApp().getLog().info('Waiting for the initial ' + AppConfig.NODES_COUNT + ' NODE_CONTROLLER components.', this._uLogID_A, 5);
      
      let compsCount: number = this.getApp().getClientComponentNetcuts().getOnlineComponentsCountFromType(ComponentType.NODE_CONTROLLER);
      if(compsCount > AppConfig.NODES_COUNT) {
        this.getApp().getLog().error('ServerComponentValidator. Error.');
        
        this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
        
        return ;
      }else if(compsCount < AppConfig.NODES_COUNT) {
        //void
      }else {
        this.getApp().getLog().info('Initial ' + AppConfig.NODES_COUNT + ' NODE_CONTROLLER components connected.');
        
        this._isInitialNodeControllersConnected = true;
      }
    }
    
    if(this._currentlyCreatingComponent) {
      let onlineCompsCount: number = this.getApp().getClientComponentNetcuts().getOnlineComponentsCountFromType(this._currentlyCreatingComponent.componentType);
      if(onlineCompsCount > 1) {
        // Currently only one component of each type can exits.
        this.getApp().getLog().error('Component creation through a NODE_CONTROLLER failed. Remote component type: ' + this._currentlyCreatingComponent.componentType.name);
        
        this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
        
        return ;
      }
      
      if(onlineCompsCount > this._currentlyCreatingComponent.curentOnlineComponentsCount) {
        this.getApp().getLog().info('Dependency connected with type: ' + this._currentlyCreatingComponent.componentType.name);
        
        this._currentlyCreatingComponent = null;
      }else {
        // Wait for 10 seconds for the component or treat as critical error.
        this._currentlyCreatingComponent.currentWaitDurationS += deltaS;
        if(this._currentlyCreatingComponent.currentWaitDurationS >= 20000) {//asd_tst_001;// 20
          this.getApp().getLog().error('Component creation through a NODE_CONTROLLER failed. Remote component type: ' + this._currentlyCreatingComponent.componentType.name);
          
          this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
          
          return ;
        }
      }
    }
    
    let isAllComponentsOnlineForWorld: boolean = true;
    // Check if any components need creating.
    for(let i: number=0;i<ServerStructureHandler.targetServerComponentsStructure.length;i++) {
      let compConfig: ServerComponentConfig = ServerStructureHandler.targetServerComponentsStructure[i];
      
      if(compConfig.isAutoCreated) {
        let onlineCompsCount: number = this.getApp().getClientComponentNetcuts().getOnlineComponentsCountFromType(compConfig.componentType);
        if(onlineCompsCount > compConfig.count) {
          this.getApp().getLog().error('ServerComponentValidator. Error.');
          
          this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
          
          return ;
        }
        
        if(onlineCompsCount < compConfig.count) {
          // More components required. Create it.
          isAllComponentsOnlineForWorld = false;
          
          if(!this._currentlyCreatingComponent) {
            if(this._isInitialNodeControllersConnected) {
              let nodeControllerNetcut: Netcut_SC_ServerManager_To_NodeController = <Netcut_SC_ServerManager_To_NodeController>this.getApp().getClientComponentNetcuts().getDefaultOnlineNetcut(ComponentType.NODE_CONTROLLER);
              if(nodeControllerNetcut) {
                this._currentlyCreatingComponent = new CurrentlyCreatingComponent(
                  compConfig.componentType, 
                  onlineCompsCount
                );
                
                nodeControllerNetcut.requestCreateComponent(compConfig.componentType);
                
                this.getApp().getLog().info('Component create request sent. Component type: ' + compConfig.componentType.name);
              }else {
                this.getApp().getLog().info('Waiting for a NODE_CONTROLLER to create a component with type: ' + compConfig.componentType.name, this._uLogID_B, 5);
              }
            }
          }
          
          break;
        }
      }
      
      let onlineForWorldCompsCount: number = this.getApp().getClientComponentNetcuts().getComponentsCountWithStatus(compConfig.componentType, ComponentStatus.ONLINE_FOR_WORLD);
      if(onlineForWorldCompsCount != compConfig.count) {
        isAllComponentsOnlineForWorld = false;
      }
    }
    
    if(!isAllComponentsOnlineForWorld && this.getApp().getServerStatus() != ServerStatus.OFFLINE) {
      // Server should be offline.
      
      this.getApp().setServerStatus(ServerStatus.OFFLINE);
    }else if(isAllComponentsOnlineForWorld && this.getApp().getServerStatus() != ServerStatus.ONLINE) {
      // Server should be online.
      
      this.getApp().setServerStatus(ServerStatus.ONLINE);
    }
  }
  
}
