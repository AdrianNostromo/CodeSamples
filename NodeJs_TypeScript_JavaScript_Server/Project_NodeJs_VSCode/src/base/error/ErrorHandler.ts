import { BasicAppComponent } from "base/component/BasicAppComponent";

import { IApp } from "base/app/IApp";
import { IErrorHandler } from "./IErrorHandler";

export class ErrorHandler extends BasicAppComponent implements IErrorHandler {

  public constructor(app: IApp) {
    super(app);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    //void
  }
  
  public error(msg: string): void {
    console.log('Exit. ' + this.getApp().getComponentType().name + ": " + 'Critical error: ' + msg);
    
    process.exit(-2);
  }
  
}
