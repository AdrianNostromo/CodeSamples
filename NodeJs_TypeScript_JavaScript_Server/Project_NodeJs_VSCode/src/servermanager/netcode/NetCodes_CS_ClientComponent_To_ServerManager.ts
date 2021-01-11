import { NetCodeRanges } from "base/common/net/netcodes/NetCodeRanges";

export class NetCodes_CS_ClientComponent_To_ServerManager {

  // ip: string
  // port: int
  public static readonly REQUEST_SYNC_COMPONENT_LISTENER_IP_PORT: number = NetCodeRanges.CS_ClientComponentToServerManager + 1;

  // componentTypeID: int
  public static readonly REQUEST_COMPONENT_LISTENER_IP_PORT_FROM_TYPE: number = NetCodeRanges.CS_ClientComponentToServerManager + 2;

  public static readonly RESPONSE_SET_SERVER_STATUS_OK: number = NetCodeRanges.CS_ClientComponentToServerManager + 3;

}
