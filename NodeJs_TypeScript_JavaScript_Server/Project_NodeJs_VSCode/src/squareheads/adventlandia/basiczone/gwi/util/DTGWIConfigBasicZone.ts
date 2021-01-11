import { DTGWIConfig } from "localbase/gwi/util/DTGWIConfig";

export class DTGWIConfigBasicZone extends DTGWIConfig {

  public constructor(serverInstanceId: number, maxPlayersCount: number) {
    super(serverInstanceId, "Based", maxPlayersCount);

    //void
  }
  
}
