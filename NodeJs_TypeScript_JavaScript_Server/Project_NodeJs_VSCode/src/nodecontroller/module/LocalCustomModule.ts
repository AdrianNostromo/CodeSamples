import { CustomModule } from "base/module/CustomModule";

import { IAppLocal } from "./../app/IAppLocal";
import { ComponentDependencyEntry } from "base/common/util/ComponentDependencyEntry";

export class LocalCustomModule extends CustomModule {

    private _app: IAppLocal;

    constructor(app: IAppLocal, componentDependenciesList: ComponentDependencyEntry[]) {
        super(app, componentDependenciesList);

        this._app = app;
    }

    public getApp(): IAppLocal {
        return this._app;
    }
  
}
