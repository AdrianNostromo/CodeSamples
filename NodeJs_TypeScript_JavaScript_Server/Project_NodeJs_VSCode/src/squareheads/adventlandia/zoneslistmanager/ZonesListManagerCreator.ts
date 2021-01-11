import { ZonesListManagerList } from "./ZonesListManagerList";
import { IAppLocal } from "squareheads/app/IAppLocal";
import { IExtenablePersistentIsolatedInstanceGWI } from "localbase/gwi/IExtenablePersistentIsolatedInstanceGWI";
import { GWIExtenablePersistentIsolatedInstance } from "localbase/gwi/GWIExtenablePersistentIsolatedInstance";
import { DTGWIConfigBasicZone } from "../basiczone/gwi/util/DTGWIConfigBasicZone";
import { GameWorldInteractorLocal } from "../basiczone/gwi/GameWorldInteractorLocal";

export class ZonesListManagerCreator extends ZonesListManagerList {

    private static readonly MAX_MATCHES_COUNT: number = 10;

    private static gameMatchIdCounter: number = 0;
  
    private static serverInstanceIdCounter: number = 0;
  /*asdr
    private adventKingsshipServersMap: Map<number, AdventKingsshipGWI> = new Map();
  */
  
    public constructor(app: IAppLocal) {
        super(app);

        //void
    }
    
    public getOrCreateOptimallyJoinableScearioServer(): IExtenablePersistentIsolatedInstanceGWI {
        let match: IExtenablePersistentIsolatedInstanceGWI = super.getOrCreateOptimallyJoinableScearioServer();
        if(match != null) {
            return match;
        }

        if(this.getActiveServersCount() >= ZonesListManagerCreator.MAX_MATCHES_COUNT) {
            return null;
        }

        // Create a new match instance if possible.
        match = this.createNewMatch();

        return match;
    }

    public createNewMatch(): GWIExtenablePersistentIsolatedInstance {
        let serverInstanceId: number = ++ZonesListManagerCreator.gameMatchIdCounter;
        
        let config: DTGWIConfigBasicZone = new DTGWIConfigBasicZone(
            serverInstanceId,
            20
        );
        
        let scenarioServer: GameWorldInteractorLocal = new GameWorldInteractorLocal(this, config);
        this.addComponent(scenarioServer);
        scenarioServer.setIsPlayEnabled(true);
        
        this.scenarioServersList.push(scenarioServer);
        this.onScenarioServerAdded(scenarioServer);

        return scenarioServer;
    }
    
}
