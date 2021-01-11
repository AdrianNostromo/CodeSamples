import { LogicError } from "base/exception/LogicError";

export class BasicModuleTemplate {

  public readonly moduleId: string;

  // If false, a module will not be created.
  public readonly hasModule: boolean;

  public constructor(moduleId: string, hasModule: boolean) {
    this.moduleId = moduleId;

    this.hasModule = hasModule;
  }

}
