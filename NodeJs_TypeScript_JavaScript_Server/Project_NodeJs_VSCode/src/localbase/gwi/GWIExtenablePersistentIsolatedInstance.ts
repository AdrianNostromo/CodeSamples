import { GWIExtendable } from "./GWIExtendable";
import { IExtenablePersistentIsolatedInstanceGWI } from "./IExtenablePersistentIsolatedInstanceGWI";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";

export abstract class GWIExtenablePersistentIsolatedInstance extends GWIExtendable implements IExtenablePersistentIsolatedInstanceGWI {

  private static readonly POSSIBLE_NEW_PLAYERS_TIMEOUT_S: number = 5;
  // This value is reseted after 5 seconds if no extra players added to it.
  private possibleNewPlayersCount: number = 0;
  private possibleNewPlayersTimoutS: number = 0;

  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }

  public getPossibleNewPlayersCount(): number {
    return this.possibleNewPlayersCount;
  }

  public incrementPossibleNewPlayersCount(): void {
    this.possibleNewPlayersCount++;
    this.possibleNewPlayersTimoutS = GWIExtenablePersistentIsolatedInstance.POSSIBLE_NEW_PLAYERS_TIMEOUT_S;
  }
  
  public appLoop_GameWorld(deltaS: number, deltaMS: number): void {
    super.appLoop_GameWorld(deltaS, deltaMS);
  
    if(this.possibleNewPlayersCount > 0) {
      this.possibleNewPlayersTimoutS -= deltaS;
      if(this.possibleNewPlayersTimoutS <= 0) {
        this.possibleNewPlayersCount = 0;
      }
    }
  }

}
