import { Module } from "localbase/gwi/entity/module/Module";
import { WorldEntityLocal } from "../WorldEntityLocal";

export class ModuleLocal extends Module {

  public constructor(moduleId: string, entity: WorldEntityLocal, usesGameLoop: boolean) {
    super(moduleId, entity, usesGameLoop);
    
    //void
  }

  public getEntity(): WorldEntityLocal {
    return <WorldEntityLocal>this.entity;
  }
  
}
