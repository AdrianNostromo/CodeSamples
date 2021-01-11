import { BasicAppComponent } from "base/component/BasicAppComponent";

import { IAppLocal } from "./../app/IAppLocal";

export class LocalAppComponent extends BasicAppComponent {

    private _app: IAppLocal;

    public constructor(app: IAppLocal) {
        super(app);

        this._app = app;
    }

    public getApp(): IAppLocal {
        return this._app;
    }
    
}
