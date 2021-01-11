import { IAppLocal } from "./app/IAppLocal";
import { SHSolarOne } from "./SHSolarOne";
import { ISquareHeadsManager } from "./ISquareHeadsManager";

export class SquareHeadsManager extends SHSolarOne implements ISquareHeadsManager {

    public constructor(app: IAppLocal) {
        super(app);

        //void
    }
  
}
