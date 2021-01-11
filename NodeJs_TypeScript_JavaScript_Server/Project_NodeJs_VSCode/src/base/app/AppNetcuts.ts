import { AppBase } from "./AppBase";
import { ComponentType } from "base/common/ComponentType";
import { AppDirs } from "base/common/util/AppDirs";
import { BasicNetcutsFactory } from "base/net/BasicNetcutsFactory";

import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentNetcutsHandler } from "base/common/net/ComponentNetcutsHandler";
import { NetcutFactoryConfig } from "base/net/util/NetcutFactoryConfig";
import { Netcut_Component_To_Component } from "base/common/net/netcut/Netcut_Component_To_Component";

export class AppNetcuts extends AppBase {

  private _netcutsFactory: BasicNetcutsFactory;

  public clientComponentNetcuts: ComponentNetcutsHandler = null;
  public serverComponentNetcuts: ComponentNetcutsHandler = null;

  public constructor(componentType: ComponentType, dirs: AppDirs, netcutsFactory: BasicNetcutsFactory) {
    super(componentType, dirs);
    
    this._netcutsFactory = netcutsFactory;
  }

  public getClientComponentNetcuts(): ComponentNetcutsHandler {
      return this.clientComponentNetcuts;
  }

  public getServerComponentNetcuts(): ComponentNetcutsHandler {
      return this.serverComponentNetcuts;
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    this.addComponent(this.clientComponentNetcuts = new ComponentNetcutsHandler(this.getApp()));
    this.getClientComponentNetcuts().setCbNetcutError(this.onComponentNetcutError.bind(this));
    
    this.addComponent(this.serverComponentNetcuts = new ComponentNetcutsHandler(this.getApp()));
    this.getServerComponentNetcuts().setCbNetcutError(this.onComponentNetcutError.bind(this));
  }
  
  protected pushNetcut(remoteComponentType: ComponentType, con: GeneralTcpConnection, isServerLocally: boolean): void {
    let newNetcutConfig: NetcutFactoryConfig = null;
    if(isServerLocally) {
      newNetcutConfig = this._netcutsFactory.getNetcutConfig(BasicNetcutsFactory.CONNECTION_TYPE_SERVER, remoteComponentType);
      if(newNetcutConfig == null && this._netcutsFactory.getNetcutConfig(BasicNetcutsFactory.CONNECTION_TYPE_SERVER, null)) {
        newNetcutConfig = this._netcutsFactory.getNetcutConfig(BasicNetcutsFactory.CONNECTION_TYPE_SERVER, null);
      }
    }else {
      newNetcutConfig = this._netcutsFactory.getNetcutConfig(BasicNetcutsFactory.CONNECTION_TYPE_CLIENT, remoteComponentType);
      if(newNetcutConfig == null && this._netcutsFactory.getNetcutConfig(BasicNetcutsFactory.CONNECTION_TYPE_CLIENT, null)) {
        newNetcutConfig = this._netcutsFactory.getNetcutConfig(BasicNetcutsFactory.CONNECTION_TYPE_CLIENT, null);
      }
    }
    if(newNetcutConfig == null) {
      this.getApp().getLog().error('Netcut error.');
      
      this.getApp().getErrorHandler().error('Netcut error.');
      
      return ;
    }
    let newNetcut: Netcut_Component_To_Component = newNetcutConfig.newInstance(this.getApp(), con, remoteComponentType);
    if(!newNetcut) {
      this.getApp().getLog().error('Netcut error.');
      
      this.getApp().getErrorHandler().error('Netcut error.');
      
      return ;
    }
    newNetcut.create();
    
    if(!isServerLocally) {
      this.getServerComponentNetcuts().addNetcut(newNetcut);
    }else {
      this.getClientComponentNetcuts().addNetcut(newNetcut);
    }
    
    newNetcut.processConData();
  }

  private onComponentNetcutError(netcut: ComponentNetcutsHandler): void {
    this.getApp().getLog().error('Netcut error.');
    
    this.getApp().getErrorHandler().error('Netcut error.');
  }

}
