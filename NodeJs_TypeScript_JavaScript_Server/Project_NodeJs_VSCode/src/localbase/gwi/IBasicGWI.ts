import { IBasicComponent } from "base/component/IBasicComponent";

import { IpPort } from "base/net/util/IpPort";
import { HandlerData } from "./util/HandlerData";

export interface IBasicGWI extends IBasicComponent {

  getServerInstanceId(): number;
  getScenarioId(): string;
  getIsInitFinished(): boolean;
  getPlayerServerIpPort(): IpPort;
  getHandlerData(): HandlerData;
  isAnyPlayerConnected(): boolean;
  
}
