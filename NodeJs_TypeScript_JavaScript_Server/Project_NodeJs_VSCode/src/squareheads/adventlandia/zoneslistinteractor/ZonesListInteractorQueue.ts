import { ZonesListInteractorQueueBase } from "./ZonesListInteractorQueueBase";
import { IAppLocal } from "squareheads/app/IAppLocal";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { QueuePlayer } from "./util/QueuePlayer";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import { IpPort } from "base/net/util/IpPort";
import { ILoopUserBase_General } from "base/component/loop/list/ILoopUserBase_General";
import { IExtenablePersistentIsolatedInstanceGWI } from "localbase/gwi/IExtenablePersistentIsolatedInstanceGWI";

class NetCodes {
  
    public static readonly SC_C_MATCH_FIND_STATUS_ON: number = 200020;
    public static readonly CS_R_MATCH_FIND_STATUS_ON_OK: number = 200021;
  
    public static readonly SC_C_MATCH_FIND_FINISHED: number = 200030;
    // int serverInstanceId
    // string serverIp
    // int serverPort
    // string scenarioId
    public static readonly CS_R_MATCH_FIND_FINISHED_OK: number = 200031;
  
}
  
export abstract class ZonesListInteractorQueue extends ZonesListInteractorQueueBase implements ILoopUserBase_General {

    public _ILoopUserBase_General: true = true;
  
    private static readonly QUEUE_CHECK_DELAY_S: number = 1.5;
    
    private queueCheckDelayS: number = 0;
  
    public constructor(app: IAppLocal) {
        super(app);

        //void
    }
    
    protected onQueuePlayerAdded(pl: QueuePlayer): void {
        super.onQueuePlayerAdded(pl);
        
        // Get the player session id.
        this.sendMsg_SC_C_MATCH_FIND_STATUS_ON(pl);
    }

    protected onProcessQueueClientMsg(pl: QueuePlayer, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
        if(super.onProcessQueueClientMsg(pl, msgLen, msgType, msgNetBuffer)) {
            return true;
        }

        switch (msgType) {
        case NetCodes.CS_R_MATCH_FIND_STATUS_ON_OK:
            if(pl.getCurrentCommandNetCode() != NetCodes.SC_C_MATCH_FIND_STATUS_ON) {
                throw new Error('Security warning.');
            }
            pl.clearCurrentCommandNetCode();

            pl.isMatchFindInProgress = true;

            break;
        case NetCodes.CS_R_MATCH_FIND_FINISHED_OK:
            if(pl.getCurrentCommandNetCode() != NetCodes.SC_C_MATCH_FIND_FINISHED) {
                throw new Error('Security warning.');
            }
            pl.clearCurrentCommandNetCode();
            
            // Con finished.
            pl.markForRemoval = true;

            break;
        default:
            return false;
        }
        
        return true;
    }
    
    private sendMsg_SC_C_MATCH_FIND_STATUS_ON(pl: QueuePlayer): void {
        pl.setCurrentCommandNetCode(NetCodes.SC_C_MATCH_FIND_STATUS_ON);
        pl.getCon().sendBlock(new NetMsgsBlock(1, 
            4)
            .startMsgWrite()
            .writeInt(NetCodes.SC_C_MATCH_FIND_STATUS_ON)
            .endMsgWrite()
        );
    }

    public appLoop_General(deltaS: number, deltaMS: number): void {
        this.appLoop_queue(deltaS, deltaMS);
    }
    
    private appLoop_queue(deltaS: number, deltaMS: number): void {
        this.queueCheckDelayS -= deltaS;
        if(this.queueCheckDelayS < 0) {
            this.queueCheckDelayS = ZonesListInteractorQueue.QUEUE_CHECK_DELAY_S;
            
            let playersList: QueuePlayer[] = this.netClientsHolder.getClientsList();
            // Need to enumerate from the start, so the older players have a higherr priority.
            for(let i: number=0;i<playersList.length;i++) {
                let pl: QueuePlayer = playersList[i];
                if(pl != null && pl.isMatchFindInProgress && pl.getCurrentCommandNetCode() < 0) {
                    // Get the first scenario server that is optimally crowded.
                    let scenarioServer: IExtenablePersistentIsolatedInstanceGWI = this.getApp().getSquareHeads().getAdventLandia().getZonesListManager().getOrCreateOptimallyJoinableScearioServer();

                    if(scenarioServer == null || !scenarioServer.getIsInitFinished()) {
                        break;
                    }

                    scenarioServer.incrementPossibleNewPlayersCount();

                    let scenarioServerIpPort: IpPort = scenarioServer.getPlayerServerIpPort();
                    
                    this.sendMsg_SC_C_MATCH_FIND_FINISHED(
                        pl, 
                        scenarioServer.getServerInstanceId(), 
                        scenarioServerIpPort.ip, 
                        scenarioServerIpPort.port,
                        scenarioServer.getScenarioId()
                    );
                }
            }
        }
    }

    // int serverInstanceId
    // string serverIp
    // int serverPort
    private sendMsg_SC_C_MATCH_FIND_FINISHED(pl: QueuePlayer, serverInstanceId: number, serverIp: string, serverPort: number, scenarioId: string): void {
        pl.setCurrentCommandNetCode(NetCodes.SC_C_MATCH_FIND_FINISHED);
        pl.getCon().sendBlock(new NetMsgsBlock(1, 
            4
            +4
            +NetMsgsBlock.stringInBufferSize(serverIp)
            +4)
            .startMsgWrite()
            .writeInt(NetCodes.SC_C_MATCH_FIND_FINISHED)
            .writeInt(serverInstanceId)
            .writeString(serverIp)
            .writeInt(serverPort)
            .writeString(scenarioId)
            .endMsgWrite()
        );
    }

}
