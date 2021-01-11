import { GWILPlayerActions } from "./GWILPlayerActions";
import { IHandler_GWI } from "localbase/gwi/IHandler_GWI";
import { DTGWIConfigBasicZone } from "./util/DTGWIConfigBasicZone";
import { BasicPlayer } from "localbase/gwi/util/BasicPlayer";
import { Player } from "./util/Player";
import { NetClient } from "localbase/gwi/util/NetClient";

export class GameWorldInteractorLocal extends GWILPlayerActions {

  public constructor(handler: IHandler_GWI, config: DTGWIConfigBasicZone) {
    super(handler, config);
    
    //void
  }

  protected createPlayerInstance(playerId: number, accountId: number): BasicPlayer {
    let pl: BasicPlayer = new Player(
      playerId,
      accountId
    );

    return pl;
  }
  
  protected onClientRemoved_pre(client: NetClient): void {
    let pl: BasicPlayer = client.player;

    super.onClientRemoved_pre(client);

    if(pl != null) {
      this.removePlayer(pl);
    }
  }

}
