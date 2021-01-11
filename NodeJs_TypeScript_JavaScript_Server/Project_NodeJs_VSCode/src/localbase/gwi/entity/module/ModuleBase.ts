import { BasicWorldEntity } from "./../BasicWorldEntity";

export class ModuleBase {
  
  public readonly moduleId: string;

  protected readonly entity: BasicWorldEntity;

  public readonly usesGameLoop: boolean;

  public constructor(moduleId: string, entity: BasicWorldEntity, usesGameLoop: boolean) {
    this.moduleId = moduleId;

    this.entity = entity;

    this.usesGameLoop = usesGameLoop;
  }

  public gameLoop(deltaS: number, deltaMS: number): void {
    //void
  }

}
