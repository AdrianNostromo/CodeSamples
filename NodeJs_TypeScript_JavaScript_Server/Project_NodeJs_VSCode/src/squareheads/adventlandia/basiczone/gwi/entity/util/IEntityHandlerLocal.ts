import { IEntityHandler } from "localbase/gwi/entity/util/IEntityHandler";

export interface IEntityHandlerLocal extends IEntityHandler {

  reserveNextNexusTeamIndex(): number;
  
}
