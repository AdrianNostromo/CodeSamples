import { SHAdventLandia } from "./SHAdventLandia";
import { IAppLocal } from "./app/IAppLocal";
import { SolarOneManager } from "./solarone/SolarOneManager";

export class SHSolarOne extends SHAdventLandia {

    public solarOne: SolarOneManager = null;

    public constructor(app: IAppLocal) {
        super(app);

        //void
    }

    protected create_framework(): void {
        super.create_framework();

        this.addComponent(this.solarOne = new SolarOneManager(this.getApp()));
    }

    public getSolarOne(): SolarOneManager {
        return this.solarOne;
    }

}
