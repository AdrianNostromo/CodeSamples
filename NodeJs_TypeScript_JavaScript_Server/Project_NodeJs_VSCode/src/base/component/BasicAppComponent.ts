import { BasicComponent } from "./BasicComponent";

import { ComponentStatus } from "base/common/util/ComponentStatus";
import { IApp } from "base/app/IApp";
import { IBasicComponent } from "./IBasicComponent";

export class BasicAppComponent extends BasicComponent {

  private app: IApp;

  public constructor(app: IApp) {
    super();
    
    this.app = app;
  }

  // This is used by the app component to set this as the app value.
  protected initApp(app: IApp): void {
      this.app = app;
  }

  public getApp(): IApp {
    return this.app;
  }
  
  public componentStatusChanged(componentStatus: ComponentStatus): void {
    let componentsList: IBasicComponent[] = this.getComponentsList();
    if(componentsList != null) {
      for(let i: number=0;i<componentsList.length;i++) {
        let rawComp: IBasicComponent = componentsList[i];
        if(rawComp != null && rawComp instanceof BasicAppComponent) {
          let comp: BasicAppComponent = <BasicAppComponent>rawComp;
          
          comp.componentStatusChanged(componentStatus);
        }
      }
    }
  }
  
  public isComponentTransientStateFinished(componentStatus: ComponentStatus): boolean {
    let componentsList: IBasicComponent[] = this.getComponentsList();
    if(componentsList != null) {
      for(let i: number=0;i<componentsList.length;i++) {
        let rawComp: IBasicComponent = componentsList[i];
        if(rawComp != null && rawComp instanceof BasicAppComponent) {
          let comp: BasicAppComponent = <BasicAppComponent>rawComp;

          let ret: boolean = comp.isComponentTransientStateFinished(componentStatus);
          if(ret === false) {
            return false;
          }else if(ret === true) {
            //void
          }else {
            throw new Error("Error.");
          }
        }
      }
    }
    
    return true;
  }
  
}
