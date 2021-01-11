import { ZonesListInteractorNetValidator } from "./ZonesListInteractorNetValidator";
import { IAppLocal } from "squareheads/app/IAppLocal";
import { QueuePlayer } from "./util/QueuePlayer";
import { NetClientsHolder } from "base/common/client/net/holder/NetClientsHolder";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export abstract class ZonesListInteractorQueueBase extends ZonesListInteractorNetValidator {

    private static readonly IDDLE_CONNECTION_THRESHOLD_S: number = 5;
    private static readonly INTER_PING_DELAY_S: number = 1;
    
    protected netClientsHolder: NetClientsHolder<QueuePlayer> = null;
    
    public constructor(app: IAppLocal) {
        super(app);

        //void
    }
        
    protected create_framework(): void {
        super.create_framework();
        
        this.addComponent(this.netClientsHolder = new NetClientsHolder<QueuePlayer>(
            null,//this.onNewClient.bind(this),
            null,//this.onClientRemoved_pre.bind(this),
            this.onProcessQueueClientMsg.bind(this)
        ));

        this.netClientsHolder.iddleDataReceiveThresholdS = ZonesListInteractorQueueBase.IDDLE_CONNECTION_THRESHOLD_S;
        this.netClientsHolder.interPingDelayS = ZonesListInteractorQueueBase.INTER_PING_DELAY_S;
    }

    protected onNetClientValidationSuccess(accountId: number, con: GeneralTcpConnection): void {
        // Enable auto crypto.
        con.enabledAutoCryptoRead();
        con.enabledAutoCryptoWrite();

        let pl: QueuePlayer = new QueuePlayer(
            con, 
            accountId
        );
        this.netClientsHolder.addClient(pl);

        pl.isPingAllowed = true;
        
        this.onQueuePlayerAdded(pl)
    }
    
    protected onQueuePlayerAdded(pl: QueuePlayer): void {
        //void
    }
    
    protected onProcessQueueClientMsg(pl: QueuePlayer, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
        return false;
    }
    
}
