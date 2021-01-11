import { BasicDatastoreModule } from "base/common/db/BasicDatastoreModule";

import { IAppLocal } from "./../app/IAppLocal";
import * as random_seed from "random-seed";

export abstract class ClientsDatastoreBase extends BasicDatastoreModule {

    private _app: IAppLocal;

    protected rng: random_seed.RandomSeed = null;

    public constructor(app: IAppLocal) {
        super(app);

        this._app = app;
    }

    public getApp(): IAppLocal {
        return this._app;
    }
    
    protected create_framework(): void {
        super.create_framework();
        
        this.rng = random_seed.create((new Date()).getTime().toString(8));
    }
    
}
