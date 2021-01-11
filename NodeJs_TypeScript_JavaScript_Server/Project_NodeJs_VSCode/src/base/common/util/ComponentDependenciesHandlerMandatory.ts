import { ComponentDependenciesHandlerBase } from "./ComponentDependenciesHandlerBase";

import { IApp } from "base/app/IApp";

import { ComponentType } from "./../ComponentType";
import { ComponentStatus } from "./../util/ComponentStatus";
import { ComponentDependencyEntry } from "./ComponentDependencyEntry";

import { ServerConnectionCallbacksHolder } from "./ServerConnectionCallbacksHolder";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { IpPort } from "base/net/util/IpPort";
import { Netcut_SC_ServerManager_To_Component } from "servermanager/netcut/Netcut_SC_ServerManager_To_Component";
import { ILoopUserBase_General } from "base/component/loop/list/ILoopUserBase_General";
import { Netcut_CS_Component_To_ServerManager } from "base/servermanager/netcut/Netcut_CS_Component_To_ServerManager";

class CurrentComponentDependency {

  public componentType: ComponentType; 
  public componentStatusLayer: ComponentStatus;
  public completionStatus: number;

  public statusDurationS: number = 0;
  public logicDelayS: number = 0;
  public ipPortResponseCurrentWaitTimeS: number = 0;
  public isUnvalidatedConnected: boolean = false;
  public isError: boolean = false;
  public isValidated: boolean = false;
  public isDepConFailed: boolean = false;

  public constructor(componentType: ComponentType, componentStatusLayer: ComponentStatus, completionStatus: number) {
    this.componentType = componentType;
    this.componentStatusLayer = componentStatusLayer;
    this.completionStatus = completionStatus;
  }

}

export class ComponentDependenciesHandlerMandatory extends ComponentDependenciesHandlerBase implements ILoopUserBase_General {
  
  public _ILoopUserBase_General: true = true;
  
  private static readonly COMPLETION_STATUS_NEW: number = 1;
  private static readonly COMPLETION_STATUS_WAITING_FOR_LISTENER_IP_PORT: number = 2;
  private static readonly COMPLETION_STATUS_WAITING_FOR_CONNECTION: number = 3;

  private _dependenciesList: ComponentDependencyEntry[] = [];
  
  private _isDepsConnected_CONNECTING = false;
  private _isDepsConnected_ONLINE_FOR_COMPONENTS = false;
  
  private _currentDependency: CurrentComponentDependency = null;
  
  private _uLogID_A: number = null;
  
  private _uniquePostDepConRetryLogger: Map<number, boolean> = new Map();
  
  private depConnectionCbHolder: ServerConnectionCallbacksHolder = new ServerConnectionCallbacksHolder(
    this.onUnvalidatedDepConnection.bind(this),
    this.onDepConnectionFail.bind(this),
    this.onDepConValidated.bind(this)
  );

  public constructor(app: IApp) {
    super(app);
    
    //void
  }
  
  public appLoop_General(deltaS: number, deltaMS: number): void {
    if(this._currentDependency) {
      this.doActiveDependencyTick(deltaS, deltaMS);
    }
    
    if(!this._currentDependency) {
      // If any dep is missing, update the component status as needed.
      if(!this.depsTick(deltaS, deltaMS, ComponentStatus.CONNECTING)) {
        this._isDepsConnected_CONNECTING = false;
        
        // Set the component status to CONNECTING if it is higher.
        if(this.getApp().getComponentStatus().id > ComponentStatus.CONNECTING.id) {
          this.getApp().setComponentStatus(ComponentStatus.CONNECTING);
        }
      }else {
        this._isDepsConnected_CONNECTING = true;
        
        if(!this.depsTick(deltaS, deltaMS, ComponentStatus.ONLINE_FOR_COMPONENTS)) {
          this._isDepsConnected_ONLINE_FOR_COMPONENTS = false;
          
          // Set the component status to ONLINE_FOR_COMPONENTS if it is higher.
          if(this.getApp().getComponentStatus().id > ComponentStatus.ONLINE_FOR_COMPONENTS.id) {
            this.getApp().setComponentStatus(ComponentStatus.ONLINE_FOR_COMPONENTS);
          }
        }else {
          this._isDepsConnected_ONLINE_FOR_COMPONENTS = true;
        }
      }
    }
  }
  
  private depsTick(deltaS: number, deltaMS: number, dependencyComponentStatus: ComponentStatus): boolean {
    for(let i: number=0;i<this._dependenciesList.length;i++) {
      let depComponent: ComponentDependencyEntry = this._dependenciesList[i];
      if(depComponent.componentStatusLayer == dependencyComponentStatus) {
        // Is CONNECTING dependency.
        if(!this.getApp().getServerComponentNetcuts().getDefaultOnlineNetcut(depComponent.componentType)) {
          // Dependency required.
          this._currentDependency = new CurrentComponentDependency(
            depComponent.componentType, 
            depComponent.componentStatusLayer,
            ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_NEW
          );
          
          return false;
        }
      }
    }
    
    return true;
  }
  
  public componentStatusChanged(componentStatus: ComponentStatus): void {
    super.componentStatusChanged(componentStatus);
    
    //void
  }
  
  public isComponentTransientStateFinished(componentStatus: ComponentStatus): boolean {
    let ret: boolean = super.isComponentTransientStateFinished(componentStatus);
    
    if(componentStatus == ComponentStatus.CONNECTING) {
      if(!this._isDepsConnected_CONNECTING) {
        ret = false;
      }
    }else if(componentStatus == ComponentStatus.ONLINE_FOR_COMPONENTS) {
      if(!this._isDepsConnected_ONLINE_FOR_COMPONENTS) {
        ret = false;
      }
    }
    
    return ret;
  }
  
  private doActiveDependencyTick(deltaS: number, deltaMS: number): boolean {
    if(!this._currentDependency) {
      return false;
    }
    
    if(this._currentDependency.isError) {
      // Connection failed, try again.
      this.getApp().getLog().warning('Server dependency connection failed. Remote server component type: ' + this._currentDependency.componentType.name + '. Treated a warning, not critical at this location. Try again.');
      
      this._currentDependency = null;
      
      return false;
    }
    
    this._currentDependency.statusDurationS += deltaS;
    
    if(this._currentDependency.logicDelayS > 0) {
      this._currentDependency.logicDelayS -= deltaS;
    }
    if(this._currentDependency.logicDelayS > 0) {
      return true;
    }

    let depListenerIpPort: IpPort;
    
    switch (this._currentDependency.completionStatus) {
      case ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_NEW:
        this._uLogID_A = this.getApp().getLog().getNextULogId();
        
        if(this._currentDependency.componentType != ComponentType.SERVER_MANAGER) {
          // Clear the dependency ip port.
          this.clearDependencyComponentListenerIpPort(this._currentDependency.componentType);
        }
        
        depListenerIpPort = this.getDependencyComponentListenerIpPort(this._currentDependency.componentType);
        if(!depListenerIpPort) {
          if(this._currentDependency.componentStatusLayer == ComponentStatus.CONNECTING || this.getApp().getComponentType() != ComponentType.SERVER_MANAGER) {
            if(this._currentDependency.componentStatusLayer == ComponentStatus.CONNECTING && this.getApp().getComponentType() == ComponentType.SERVER_MANAGER) {
              return false;
            }
            
            // Make request to the ServerManager for the ip port of the dependency components listener.
            let serverManagerNetcut: Netcut_CS_Component_To_ServerManager = <Netcut_CS_Component_To_ServerManager>this.getApp().getServerComponentNetcuts().getDefaultOnlineNetcut(ComponentType.SERVER_MANAGER);
            if(serverManagerNetcut == null) {
              return false;
            }
            if(!serverManagerNetcut.requestComponentListenerIpPort(this._currentDependency.componentType)) {
              return false;
            }
            
            this._currentDependency.ipPortResponseCurrentWaitTimeS = 0;
            
            this._currentDependency.completionStatus = ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_LISTENER_IP_PORT;
            this._currentDependency.statusDurationS = 0;
          }else if(this._currentDependency.componentStatusLayer == ComponentStatus.ONLINE_FOR_COMPONENTS) {
            // this._currentDependency.componentStatusLayer == ComponentStatus.ONLINE_FOR_COMPONENTS && is server Manager;
            // For pos online dependencies, wait until they have their component listener active.
            
            if(this.getApp().getComponentType() != ComponentType.SERVER_MANAGER) {
              return false;
            }
            
            // Try to get the ipPort from the netcutsHandler and push it locally.
            let targetComponentNetcut: Netcut_SC_ServerManager_To_Component = <Netcut_SC_ServerManager_To_Component>this.getApp().getClientComponentNetcuts().getDefaultOnlineNetcut(this._currentDependency.componentType);
            if(!targetComponentNetcut) {
              this.sendLogPostDependencyNotOnline(this._currentDependency.componentType);
              
              this._currentDependency.logicDelayS = 0.5;
            }else {
              let ipPort: IpPort = targetComponentNetcut.getRemoteComponentListenerIpPort();
              if(!ipPort) {
                return false;
              }
              
              if(!this.getApp().getComponentDependenciesHandler().addDependencyComponentListenerIpPort(this._currentDependency.componentType, ipPort)) {
                return false;
              }
              
              this._currentDependency.completionStatus = ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_LISTENER_IP_PORT;
              this._currentDependency.statusDurationS = 0;
            }
          }
        }else {
          this._currentDependency.completionStatus = ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_LISTENER_IP_PORT;
          this._currentDependency.statusDurationS = 0;
        }
        
        break;
      case ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_LISTENER_IP_PORT:
        depListenerIpPort = this.getDependencyComponentListenerIpPort(this._currentDependency.componentType);
        if(!depListenerIpPort) {
          this._currentDependency.ipPortResponseCurrentWaitTimeS += deltaS;
          if(this._currentDependency.ipPortResponseCurrentWaitTimeS >= 5) {
            if(this._currentDependency.componentStatusLayer == ComponentStatus.CONNECTING) {
              // Ip port should have been received from the server Manager.
              return false;
            }else {
              // Dependency failed, try again.
              this.sendLogPostDependencyNotOnline(this._currentDependency.componentType);
              
              this._currentDependency = null;
              
              return false;
            }
          }
        }else {
          // Connect to the dependency component and wait until it is validated.
          let ret: boolean = this.getApp().connectToServerComponent(this._currentDependency.componentType, depListenerIpPort.ip, depListenerIpPort.port, this.depConnectionCbHolder);
          if(!ret) {
            return false;
          }
          
          this._currentDependency.completionStatus = ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_CONNECTION;
          this._currentDependency.statusDurationS = 0;
        }
        
        break;
      case ComponentDependenciesHandlerMandatory.COMPLETION_STATUS_WAITING_FOR_CONNECTION:
        // Error if not connected after 10 seconds. Show a log every 10 seconds while waiting.
        if(this.getApp().getServerComponentNetcuts().getDefaultOnlineNetcut(this._currentDependency.componentType)) {
          this._currentDependency = null;
        }else {
          if(this._currentDependency.statusDurationS > 5 && !this._currentDependency.isError && !this._currentDependency.isUnvalidatedConnected && !this._currentDependency.isValidated) {
            // Logic error.
            
            this.getApp().getLog().error('Dependency validation timeout threshold occured. Component type: ' + this._currentDependency.componentType.name);
            
            this.getApp().getErrorHandler().error('Dependency failed.');
            
            throw new Error("Error");
          }else if(this._currentDependency.statusDurationS > 200) {
            this.getApp().getLog().error('Dependency connected but not online. Component type: ' + this._currentDependency.componentType.name);
            
            this.getApp().getErrorHandler().error('Dependency failed.');
            
            throw new Error("Error");
          }else {
            this.getApp().getLog().info('Waiting for dependency with type: ' + this._currentDependency.componentType.name, this._uLogID_A, 5);
          }
        }
        
        break;
      default:
        this.getApp().getLog().error('Dependency connected but not online. Component type: ' + this._currentDependency.componentType.name);
        
        this.getApp().getErrorHandler().error('Dependency failed.');
        
        throw new Error("Error");
    }

    return true;
  }
  
  private onUnvalidatedDepConnection(con: GeneralTcpConnection, remoteComponentType: ComponentType): void {
    // Check the component type;
    if(!this._currentDependency || this._currentDependency.componentType != remoteComponentType) {
      this.getApp().getLog().error('Dependency failed. Component type: ' + remoteComponentType.name);
      
      this.getApp().getErrorHandler().error('Dependency failed.');
      
      return ;
    }
    
    this._currentDependency.isUnvalidatedConnected = true;
    
    this.getApp().getLog().info('Unvalidated dependency connected with type: ' + this._currentDependency.componentType.name);
  }
  
  private onDepConnectionFail(con: GeneralTcpConnection, remoteComponentType: ComponentType): void {
    // Check the component type;
    if(!this._currentDependency || this._currentDependency.componentType != remoteComponentType) {
      this.getApp().getLog().error('Dependency failed. Component type: ' + remoteComponentType.name);
      
      this.getApp().getErrorHandler().error('Dependency failed.');
      
      return ;
    }
    
    this._currentDependency.isError = true;
    
    this.getApp().getLog().info('Unvalidated dependency connection failed with type: ' + this._currentDependency.componentType.name);
  }
  
  private onDepConValidated(con: GeneralTcpConnection, remoteComponentType: ComponentType): void {
    // Check the component type;
    if(!this._currentDependency || this._currentDependency.componentType != remoteComponentType) {
      this.getApp().getLog().error('Dependency failed. Component type: ' + remoteComponentType.name);
      
      this.getApp().getErrorHandler().error('Dependency failed.');
      
      return ;
    }
    
    this._currentDependency.isValidated = true;
    
    this.getApp().getLog().info('Dependency connection validated with type: ' + this._currentDependency.componentType.name);
  }
  
  private sendLogPostDependencyNotOnline(dependencyComponentType: ComponentType): void {
    if(!this._uniquePostDepConRetryLogger.get(dependencyComponentType.id)) {
      this._uniquePostDepConRetryLogger.set(dependencyComponentType.id, true);
      
      this.getApp().getLog().info('Post dependency no online. Dependency type: ' + this._currentDependency.componentType.name);
    }
  }
  
  public addDependency(componentType: ComponentType, componentStatusLayer: ComponentStatus): boolean {
    if(this.isComponentDependency(componentType)) {
      return false;
    }
    
    this._dependenciesList.push(new ComponentDependencyEntry(
      componentType, 
      componentStatusLayer
    ));
    
    return true;
  }
  
  public isComponentDependency(componentType: ComponentType): boolean {
    for(let i: number=0;i<this._dependenciesList.length;i++) {
      let depComponent: ComponentDependencyEntry = this._dependenciesList[i];
      if(depComponent && depComponent.componentType == componentType) {
        return true;
      }
    }
    
    return false;
  }
  
}
