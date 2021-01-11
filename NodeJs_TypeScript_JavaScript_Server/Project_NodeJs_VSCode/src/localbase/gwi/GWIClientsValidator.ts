import { GWINetListener } from "./GWINetListener";
import { RegisteredNetClientsSessionValidator } from "squareheads/net/validator/RegisteredNetClientsSessionValidator";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";

export abstract class GWIClientsValidator extends GWINetListener {
  
  private netClientsValidator: RegisteredNetClientsSessionValidator = null;
  
  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super(handler, config);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    this.addComponent(this.netClientsValidator = new RegisteredNetClientsSessionValidator(
      this.onNetClientValidationSuccess.bind(this),
      this.handler.getClientDevicesManager(),
      this.handler.getClientSessionsManager()
    ));
    this.netClientsValidator.name = "GameWorld_" + this.getServerInstanceId() + "_Validator";
  }

  // extendable;
  protected abstract onNetClientValidationSuccess(accountId: number, con: GeneralTcpConnection): void;

  protected onNewUnvalidatedCon(con: GeneralTcpConnection): void {
    this.netClientsValidator.addClientCon(con);
  }

}
