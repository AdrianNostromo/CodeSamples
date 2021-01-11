import { LocalCustomModule } from "./module/LocalCustomModule";

import { IAppLocal } from "./app/IAppLocal";

export class LogManager extends LocalCustomModule {

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
    
    //void
  }
  
}
