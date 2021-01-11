import { BasicAppComponent } from "base/component/BasicAppComponent";

import { AppDirs } from "base/common/util/AppDirs";
import { ServerStatus } from "base/common/server/ServerStatus";
import { ComponentType } from "base/common/ComponentType";
import { ComponentStatus } from "base/common/util/ComponentStatus";
import { Logger } from "base/log/Logger";
import { ErrorHandler } from "base/error/ErrorHandler";
import { ComponentDependenciesHandler } from "base/common/util/ComponentDependenciesHandler";
import { LoopDistribution } from "base/component/loop/LoopDistribution";
import { ILoopUserBase_General } from "base/component/loop/list/ILoopUserBase_General";
import { ConsoleHandler } from "base/console/ConsoleHandler";
import { ILogger } from "base/log/ILogger";

export class AppBase extends BasicAppComponent implements ILoopUserBase_General {

  public _ILoopUserBase_General: true = true;

  protected dirs: AppDirs;

  protected _componentType: ComponentType;

  private _componentStatus: ComponentStatus = ComponentStatus.OFFLINE;

  private _serverStatus: ServerStatus = ServerStatus.OFFLINE;

  private log: Logger = null;
  private errorHandler: ErrorHandler = null;
  private consoleHandler: ConsoleHandler = null;
  private componentDependenciesHandler: ComponentDependenciesHandler = null;

  private _statusChangeCheckDelayS = 0;

  public readonly loopDistribution: LoopDistribution = new LoopDistribution();
  
  public constructor(componentType: ComponentType, dirs: AppDirs) {
    super(null);

    this.dirs = dirs;
    
    this._componentType = componentType;

    this.setRoot(this);
  }

  public getDirs(): AppDirs {
    return this.dirs;
  }

  public getLog(): ILogger {
      return this.log;
  }

  public getConsoleHandler(): ConsoleHandler {
      return this.consoleHandler;
  }

  public getComponentDependenciesHandler(): ComponentDependenciesHandler {
      return this.componentDependenciesHandler;
  }

  public getErrorHandler(): ErrorHandler {
      return this.errorHandler;
  }

  protected create_framework(): void {
    super.create_framework();
    
    this.loopDistribution.setRootComponent(this);
    
    this.addComponent(this.log = new Logger(this.getApp()));
    
    this.addComponent(this.errorHandler = new ErrorHandler(this.getApp()));
    this.addComponent(this.consoleHandler = new ConsoleHandler(this.getApp()));
    
    this.addComponent(this.componentDependenciesHandler = new ComponentDependenciesHandler(this.getApp()));
  }

  public appLoop_General(deltaS: number, deltaMS: number): void {
    if(this._componentStatus.isTransientState) {
      this._statusChangeCheckDelayS -= deltaS;
      
      if(this._statusChangeCheckDelayS <= 0) {
        this._statusChangeCheckDelayS = 0.2;
        
        if(this.isComponentTransientStateFinished(this._componentStatus)) {
          if(this._componentStatus == ComponentStatus.STARTING) {
            this.setComponentStatus(ComponentStatus.CONNECTING);
          }else if(this._componentStatus == ComponentStatus.CONNECTING) {
            this.setComponentStatus(ComponentStatus.ONLINE_FOR_COMPONENTS);
          }else if(this._componentStatus == ComponentStatus.ONLINE_FOR_COMPONENTS) {
            this.setComponentStatus(ComponentStatus.ONLINE_FOR_WORLD);
          }else if(this._componentStatus == ComponentStatus.STOPING) {
            this.setComponentStatus(ComponentStatus.OFFLINE);
          }else {
            this.getApp().getLog().error('ServerComponentValidator. Error.');
            
            this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
            
            return ;
          }
        }
      }
    }
  }
  
  public getComponentType(): ComponentType {
    return this._componentType;
  }
  
  public getComponentStatus(): ComponentStatus {
    return this._componentStatus;
  }
  
  public isStatus_OFFLINE(): boolean {
    if(this._componentStatus == ComponentStatus.OFFLINE) {
      return true;
    }
    
    return false;
  }
  
  public isStatus_STOPING(): boolean {
    if(this._componentStatus == ComponentStatus.STOPING) {
      return true;
    }
    
    return false;
  }
  
  public isStatus_STARTING(): boolean {
    if(this._componentStatus == ComponentStatus.STARTING) {
      return true;
    }
    
    return false;
  }
  
  public isStatus_CONNECTING(): boolean {
    if(this._componentStatus == ComponentStatus.CONNECTING) {
      return true;
    }
    
    return false;
  }
  
  public isStatus_ONLINE_FOR_COMPONENTS(): boolean {
    if(this._componentStatus == ComponentStatus.ONLINE_FOR_COMPONENTS) {
      return true;
    }
    
    return false;
  }
  
  public isStatus_ONLINE_FOR_WORLD(): boolean {
    if(this._componentStatus == ComponentStatus.ONLINE_FOR_WORLD) {
      return true;
    }
    
    return false;
  }
  
  public isServerStatus_OFFLINE(): boolean {
    if(this._serverStatus == ServerStatus.OFFLINE) {
      return true;
    }
    
    return false;
  }
  
  public isServerStatus_ONLINE(): boolean {
    if(this._serverStatus == ServerStatus.ONLINE) {
      return true;
    }
    
    return false;
  }
  
  public setComponentStatus(componentStatus: ComponentStatus): void {
    if(this._componentStatus == componentStatus) {
      this.getApp().getLog().error('App. Error.');

      this.getApp().getErrorHandler().error('App. Error.');
      
      return ;
    }
    
    this.getLog().console('Status changed from: ' + this._componentStatus.name + ' to: ' + componentStatus.name);
    
    this._componentStatus = componentStatus;
    
    this.componentStatusChanged(this._componentStatus);
    
    if(this._componentStatus.isTransientState) {
      this._statusChangeCheckDelayS = 0.2;
    }
  }
  
  public getServerStatus(): ServerStatus {
    return this._serverStatus;
  }
  
  public setServerStatus(serverStatus: ServerStatus): void {
    this._serverStatus = serverStatus;
    
    this.getApp().getLog().info('Server status changed: ' + this._serverStatus.name);
  }
  
}
