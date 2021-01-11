import { ZonesListInteractorNetListener } from "./ZonesListInteractorNetListener";
import { IAppLocal } from "squareheads/app/IAppLocal";
import { RegisteredNetClientsSessionValidator } from "squareheads/net/validator/RegisteredNetClientsSessionValidator";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";

export abstract class ZonesListInteractorNetValidator extends ZonesListInteractorNetListener {

    private netClientsValidator: RegisteredNetClientsSessionValidator = null;
  
    public constructor(app: IAppLocal) {
        super(app);

        //void
    }
    
    protected create_framework(): void {
        super.create_framework();
        
        this.addComponent(this.netClientsValidator = new RegisteredNetClientsSessionValidator(
            this.onNetClientValidationSuccess.bind(this),
            this.getApp().getSquareHeads().getClientDevicesManager(),
            this.getApp().getSquareHeads().getClientSessionsManager()
        ));
    }

    // extendable;
    protected abstract onNetClientValidationSuccess(accountId: number, con: GeneralTcpConnection): void;

    protected onNewUnvalidatedCon(con: GeneralTcpConnection): void {
        this.netClientsValidator.addClientCon(con);
    }

}
