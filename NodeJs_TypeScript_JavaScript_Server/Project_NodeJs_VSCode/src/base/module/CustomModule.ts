import { BasicAppComponent } from "base/component/BasicAppComponent";
import { IApp } from "base/app/IApp";

import { ComponentStatus } from "base/common/util/ComponentStatus";
import { ComponentDependencyEntry } from "base/common/util/ComponentDependencyEntry";

export class CustomModule extends BasicAppComponent {

  private componentDependenciesList: ComponentDependencyEntry[];

  constructor(app: IApp, componentDependenciesList: ComponentDependencyEntry[]) {
    super(app);

    this.componentDependenciesList = componentDependenciesList;
  }

  componentStatusChanged(componentStatus: ComponentStatus) {
    super.componentStatusChanged(componentStatus);
    
    if(componentStatus == ComponentStatus.STARTING) {
      for(let i: number=0;i<this.componentDependenciesList.length;i++) {
        let compDepCfg: ComponentDependencyEntry = this.componentDependenciesList[i];

        if(!this.getApp().getComponentDependenciesHandler().addDependency(compDepCfg.componentType, compDepCfg.componentStatusLayer)) {
          this.getApp().getLog().error('Exit. Initialize error 1.');

          this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
          
          return ;
        }
      }
    }
  }

}
