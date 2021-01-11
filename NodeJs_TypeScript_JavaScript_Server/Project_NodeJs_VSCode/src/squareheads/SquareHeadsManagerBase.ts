import { IAppLocal } from "./app/IAppLocal";

import { EntryPointInteractor } from "./entrypointinteractor/EntryPointInteractor";
import { LocalCustomModule } from "./module/LocalCustomModule";

export class SquareHeadsManagerBase extends LocalCustomModule {

  public entryPointInteractor: EntryPointInteractor = null;
  
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
    
    this.addComponent(this.entryPointInteractor = new EntryPointInteractor(this.getApp()));
  }
  
}
