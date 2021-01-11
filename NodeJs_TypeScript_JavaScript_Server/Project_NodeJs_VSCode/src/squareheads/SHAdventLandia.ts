import { SquareHeadsManagerUsers } from "./SquareHeadsManagerUsers";
import { IAppLocal } from "./app/IAppLocal";
import { AdventLandiaManager } from "./adventlandia/AdventLandiaManager";

export class SHAdventLandia extends SquareHeadsManagerUsers {

    public adventLandia: AdventLandiaManager = null;

    public constructor(app: IAppLocal) {
        super(app);

        //void
    }

    protected create_framework(): void {
        super.create_framework();

        this.addComponent(this.adventLandia = new AdventLandiaManager(this.getApp()));
    }

    public getAdventLandia(): AdventLandiaManager {
        return this.adventLandia;
    }

}
