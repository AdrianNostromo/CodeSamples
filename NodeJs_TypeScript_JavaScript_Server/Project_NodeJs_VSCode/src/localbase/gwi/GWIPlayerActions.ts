import { GWISync } from "./GWISync";
import { BasicAction } from "./action/BasicAction";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";
import { BasicPlayer } from "./util/BasicPlayer";
import { SecurityError } from "base/exception/SecurityError";
import { GlobalAccessLogger } from "base/log/GlobalAccessLogger";
import { LogicError } from "base/exception/LogicError";

export abstract class GWIPlayerActions extends GWISync {

  private receivedPlayerActions: BasicAction[] = [];

  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }

  protected onPlayerRemove_pre(pl: BasicPlayer) {
    super.onPlayerRemove_pre(pl);

    for(let i: number=0;i<this.receivedPlayerActions.length;i++) {
      let action: BasicAction = this.receivedPlayerActions[i];
      
      if(!action.isProcessed && action.pl == pl) {
        action.isProcessed = true;
      }
    }
  }

  protected pushPlayerAction(pl: BasicPlayer, action: BasicAction): void {
    action.pl = pl;

    this.receivedPlayerActions.push(action);
  }

  protected gameLoop_netInputs(deltaS: number, deltaMS: number): void {
    super.gameLoop_netInputs(deltaS, deltaMS);
    
    if(this.receivedPlayerActions.length > 0) {
      for(let i: number=0;i<this.receivedPlayerActions.length;i++) {
        let action: BasicAction = this.receivedPlayerActions[i];
        if(action.isProcessed) {
          continue;
        }

        action.isProcessed = true;

        // Ignore action if the player is removed.
        if(action.pl.isRemoved) {
          continue;
        }

        // Use exception handling and automatic player removal on certain exceptions.
        try {
          this.processPlayerAction(action.pl, action);
        }catch(x) {
          if(x instanceof SecurityError) {
            GlobalAccessLogger.getLog().security('Removing client. Security error: ' + x.message);

            this.removePlayer(action.pl);
          }else if(x instanceof LogicError) {
            this.logicError("Error.processPlayerAction.1.");
          }else {
            this.logicError("Error.processPlayerAction.1.");
          }
        }
      }

      this.receivedPlayerActions = [];
    }
  }

  protected abstract processPlayerAction(pl: BasicPlayer, action: BasicAction): void;

}
