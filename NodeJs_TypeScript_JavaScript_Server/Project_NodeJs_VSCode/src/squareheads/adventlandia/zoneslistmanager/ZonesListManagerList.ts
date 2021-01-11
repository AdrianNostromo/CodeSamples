import { ZonesListManagerBase } from "./ZonesListManagerBase";
import { IAppLocal } from "squareheads/app/IAppLocal";
import { IBasicGWI } from "localbase/gwi/IBasicGWI";
import { ILogger } from "base/log/ILogger";
import { GWIExtenablePersistentIsolatedInstance } from "localbase/gwi/GWIExtenablePersistentIsolatedInstance";
import { IExtenablePersistentIsolatedInstanceGWI } from "localbase/gwi/IExtenablePersistentIsolatedInstanceGWI";
import { ClientSessionsManager } from "squareheads/clientsessionsmanager/ClientSessionsManager";
import { ClientDevicesManager } from "squareheads/clientdevicesmanager/ClientDevicesManager";
import { IHandler_GWI } from "localbase/gwi/IHandler_GWI";
import { ILoopUserBase_General } from "base/component/loop/list/ILoopUserBase_General";

export class ZonesListManagerList extends ZonesListManagerBase implements IHandler_GWI, ILoopUserBase_General {

    public _ILoopUserBase_General: true = true;
  
    private static readonly IDDLE_SERVER_REMOVAL_THRESHOLD_S: number = 15;
  
    // The optimal number of players per match.
    // The server limit should be around +20.
  
    private static readonly OPTIMAL_MATCH_PLAYERS: number = 10;
  
    protected scenarioServersList: IBasicGWI[] = [];
    private activeServersCount: number = 0;
  
    public constructor(app: IAppLocal) {
        super(app);

        //void
    }
    
    public onNetError(target: IBasicGWI): void {
        // Show a log.
        this.getApp().getLog().debug('Scenario server. Net error occured.');

        // Dispose the match.
        this.removeScenarioServer(target);
    }

    public onLogicError(target: IBasicGWI): void {
        // Show a log.
        this.getApp().getLog().debug('Scenario server. Logic error occured.');
        
        // Dispose the match.
        this.removeScenarioServer(target);
    }

    public getClientDevicesManager(): ClientDevicesManager {
        return this.getApp().getSquareHeads().getClientDevicesManager();
    }

    public getClientSessionsManager(): ClientSessionsManager {
        return this.getApp().getSquareHeads().getClientSessionsManager();
    }

    public getLog(): ILogger {
        return this.getApp().getLog();
    }

    public getOrCreateOptimallyJoinableScearioServer(): IExtenablePersistentIsolatedInstanceGWI {
        let optimalMatch: GWIExtenablePersistentIsolatedInstance = null;
        let optimalMatchFreeSlots: number;

        let initialisingMatch: GWIExtenablePersistentIsolatedInstance = null;

        // Optimisation: each time a match is sent to the player, increment a possibleNewPLayersCount with a timout of 10 seconds.
        // This will handle overcrouding.
        for(let i: number=this.scenarioServersList.length - 1;i>=0;i--) {
            if(!(this.scenarioServersList[i] instanceof GWIExtenablePersistentIsolatedInstance)) {
                continue;
            }
            let match: GWIExtenablePersistentIsolatedInstance = <GWIExtenablePersistentIsolatedInstance>this.scenarioServersList[i];
            if(match != null) {
                if(match.getIsInitFinished) {
                    if((match.getPlayersCount() + match.getPossibleNewPlayersCount()) < ZonesListManagerList.OPTIMAL_MATCH_PLAYERS) {
                        let playersDiff = ZonesListManagerList.OPTIMAL_MATCH_PLAYERS - (match.getPlayersCount() + match.getPossibleNewPlayersCount());
                        if(optimalMatch == null || playersDiff < optimalMatchFreeSlots) {
                        optimalMatch = match;
                        }
                    }
                }else {
                    if(initialisingMatch == null) {
                        initialisingMatch = match;
                    }
                }
            }
        }

        if(optimalMatch == null && initialisingMatch != null) {
            optimalMatch = initialisingMatch;
        }

        return optimalMatch;
    }

    public appLoop_General(deltaS: number, deltaMS: number): void {
        this.appLoop_local(deltaS, deltaMS);
    }
    
    private appLoop_local(deltaS: number, deltaMS: number): void {
        if(!this.getApp().getComponentStatus().isOnline) {
            return ;
        }
        
        for(let i: number=this.scenarioServersList.length - 1;i>=0;i--) {
            let match: IBasicGWI = this.scenarioServersList[i];
            if(match != null) {
                if(match.isAnyPlayerConnected()) {
                    match.getHandlerData().iddleTimeS = 0;
                }else {
                    match.getHandlerData().iddleTimeS += deltaS;
                    if(match.getHandlerData().iddleTimeS >= ZonesListManagerList.IDDLE_SERVER_REMOVAL_THRESHOLD_S) {
                        this.getApp().getLog().debug('Removing iddle scenario server.');

                        this.removeScenarioServer(match);

                        continue;
                    }
                }
            }
        }
    }

    protected getActiveServersCount(): number {
        return this.activeServersCount;
    }

    protected removeScenarioServer(gameMatch: IBasicGWI): void {
        this.getApp().getLog().debug('Match instance removed with id: ' + gameMatch.getServerInstanceId());

        let index: number = this.scenarioServersList.indexOf(gameMatch);
        if(index < 0) {
            this.getApp().getLog().error('ServerComponentValidator. Error.');

            this.getApp().getErrorHandler().error('ServerComponentValidator. Error.');
            
            return ;
        }

        this.onScenarioServerRemoved_pre(gameMatch);
        
        this.disposeComponent(gameMatch);
        
        this.scenarioServersList.splice(index, 1);
    }

    protected onScenarioServerAdded(scenarioServer: IBasicGWI): void {
        this.activeServersCount++;

        this.getApp().getLog().debug("Scenario server created. Id: " + scenarioServer.getServerInstanceId() + ". Total scenario servers count: " + this.activeServersCount);
    }

    protected onScenarioServerRemoved_pre(scenarioServer: IBasicGWI): void {
        this.activeServersCount--;

        this.getApp().getLog().debug("Scenario server removed. Id: " + scenarioServer.getServerInstanceId() + ". Total scenario servers count: " + this.activeServersCount);
    }

    protected gameWorldError(target: IBasicGWI, err: Error): void {
        this.getApp().getLog().error('Game match error occured.');
        
        this.removeScenarioServer(target);
    }

}
