import { ComponentDependenciesHandlerMandatory } from "./ComponentDependenciesHandlerMandatory";
import { IApp } from "base/app/IApp";
import { IComponentDependenciesHandler } from "./IComponentDependenciesHandler";

export class ComponentDependenciesHandler extends ComponentDependenciesHandlerMandatory implements IComponentDependenciesHandler {

  public constructor(app: IApp) {
    super(app);
    
    //void
  }
  
}
