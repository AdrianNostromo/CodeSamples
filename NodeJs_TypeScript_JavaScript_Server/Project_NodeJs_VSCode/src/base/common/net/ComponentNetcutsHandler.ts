import { BasicAppComponent } from "base/component/BasicAppComponent";
import { IApp } from "base/app/IApp";

import { ComponentType } from "base/common/ComponentType";
import { ComponentStatus } from "base/common/util/ComponentStatus";
import { Netcut_Component_To_Component } from "base/common/net/netcut/Netcut_Component_To_Component";
import { ILoopUserBase_General } from "base/component/loop/list/ILoopUserBase_General";

export class ComponentNetcutsHandler extends BasicAppComponent implements ILoopUserBase_General {
  
  public _ILoopUserBase_General: true = true;
  
  private _netcutsList: Netcut_Component_To_Component[] = [];
  private _defaultNetcuts: Map<number, Netcut_Component_To_Component> = new Map();

  private _bf_onNetcutError: (netcut: Netcut_Component_To_Component) => void = null;
  private _bf_onNetcutRemoteComponentStatusChanged: (target: Netcut_Component_To_Component) => void = null;

  private _cbNetcutError: (netcut: ComponentNetcutsHandler) => void = null;

  public constructor(app: IApp) {
    super(app);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    this._bf_onNetcutError = this.onNetcutError.bind(this);
    this._bf_onNetcutRemoteComponentStatusChanged = this.onNetcutRemoteComponentStatusChanged.bind(this);
  }
  
  public addNetcut(netcut: Netcut_Component_To_Component): void {
    netcut.setCbError(this._bf_onNetcutError);
    netcut.setCbRemoteComponentStatusChanged(this._bf_onNetcutRemoteComponentStatusChanged);
    
    this._netcutsList.push(netcut);
    
    this.updateIfNeededDefaultOnlineComponentForComponentType(netcut.getComponentType());
  }
  
  private updateIfNeededDefaultOnlineComponentForComponentType(remoteComponentType: ComponentType): void {
    let defaultNetcut: Netcut_Component_To_Component = this._defaultNetcuts.get(remoteComponentType.id);
    
    if(defaultNetcut && this._netcutsList.indexOf(defaultNetcut) >= 0 && defaultNetcut.getRemoteComponentStatus() && defaultNetcut.getRemoteComponentStatus().isOnline) {
      return ;
    }
    
    this._defaultNetcuts.delete(remoteComponentType.id);
    
    for(let i: number=0;i<this._netcutsList.length;i++) {
      let netcut: Netcut_Component_To_Component = this._netcutsList[i];
      if(netcut && netcut.getComponentType() == remoteComponentType && netcut.getRemoteComponentStatus() && netcut.getRemoteComponentStatus().isOnline) {
        this._defaultNetcuts.set(netcut.getComponentType().id, netcut);
        
        break;
      }
    }
  }
  
  private onNetcutError(netcut: Netcut_Component_To_Component): void {
    let compType: ComponentType = netcut.getComponentType();
    
    if(netcut.getComponentType() == ComponentType.SERVER_MANAGER && !netcut.getIsServerLocally()) {
      // Critical error.
      this.getApp().getLog().error('Critical error. Server Manager connection lost.');
      
      this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
      
      return null;
    }else {
      // Show a warning message and try to recover.
      if(!netcut.getIsServerLocally()) {
        this.getApp().getLog().warning('Remote server component connection lost. Try to recover. Remote component type: ' + netcut.getComponentType().name);
      }else {
        this.getApp().getLog().warning('Remote client component connection lost. Try to recover. Remote component type: ' + netcut.getComponentType().name);
      }
    }
    
    // Remove netcut.
    let index: number = this._netcutsList.indexOf(netcut);
    if (index < 0) {
      // Already processed, ignore.
      
      return ;
    }
    this._netcutsList.splice(index, 1);
    
    this.updateIfNeededDefaultOnlineComponentForComponentType(compType);
  }
  
  private onNetcutRemoteComponentStatusChanged(netcut: Netcut_Component_To_Component): void {
    this.updateIfNeededDefaultOnlineComponentForComponentType(netcut.getComponentType());
  }
  
  public getDefaultOnlineNetcut(remoteComponentType: ComponentType): Netcut_Component_To_Component {
    if(!remoteComponentType) {
      this.getApp().getLog().error('ServerComponentValidator. Error.');
      
      this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
      
      return null;
    }
    
    return this._defaultNetcuts.get(remoteComponentType.id);
  }
  
  public getOnlineComponentsCountFromType(remoteComponentType: ComponentType): number {
    if(!remoteComponentType) {
      this.getApp().getLog().error('ServerComponentValidator. Error.');
      
      this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
      
      return null;
    }
    
    let ct: number = 0;
    
    for(let i: number=0;i<this._netcutsList.length;i++) {
      let netcut: Netcut_Component_To_Component = this._netcutsList[i];
      
      if(netcut && netcut.getComponentType() == remoteComponentType && netcut.getRemoteComponentStatus() && netcut.getRemoteComponentStatus().isOnline) {
        ct++;
      }
    }
    
    return ct;
  }
  
  public getComponentsCountWithStatus(remoteComponentType: ComponentType, remoteComponentStatus: ComponentStatus): number {
    if(!remoteComponentType || !remoteComponentStatus) {
      this.getApp().getLog().error('ServerComponentValidator. Error.');
      
      this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
      
      return null;
    }
    
    let ct: number = 0;
    
    for(let i: number=0;i<this._netcutsList.length;i++) {
      let netcut: Netcut_Component_To_Component = this._netcutsList[i];
      
      if(netcut && netcut.getComponentType() == remoteComponentType && netcut.getRemoteComponentStatus() && netcut.getRemoteComponentStatus() == remoteComponentStatus) {
        ct++;
      }
    }
    
    return ct;
  }
  
  public appLoop_General(deltaS: number, deltaMS: number): void {
    for(let i: number=this._netcutsList.length - 1;i>=0;i--) {
      let netcut: Netcut_Component_To_Component = this._netcutsList[i];
      if(netcut) {
        netcut.logicLoop(deltaS, deltaMS);
      }
    }
  }
  
  public isAllComponentsOnlineForWorld(): boolean {
    for(let i: number=0;i<this._netcutsList.length;i++) {
      let netcut: Netcut_Component_To_Component = this._netcutsList[i];
      
      if(netcut && (!netcut.getRemoteComponentStatus() || netcut.getRemoteComponentStatus() != ComponentStatus.ONLINE_FOR_WORLD)) {
        return false;
      }
    }
    
    return true;
  }
  
  public setCbNetcutError(cbNetcutError: (netcut: ComponentNetcutsHandler) => void): void {
    this._cbNetcutError = cbNetcutError;
  }
  
}
