import { LocalCustomModule } from "./module/LocalCustomModule";

import { IAppLocal } from "./app/IAppLocal";

import { ComponentsProcessHandler } from "nodecontroller/util/ComponentsProcessHandler";

export class NodeController extends LocalCustomModule {

  public componentsProcessHandler: ComponentsProcessHandler = null;

  public constructor(app: IAppLocal) {
    super(
      app, 
      [
        //void
      ]
    );
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    this.addComponent(this.componentsProcessHandler = new ComponentsProcessHandler(this.getApp()));
  }
  
}
