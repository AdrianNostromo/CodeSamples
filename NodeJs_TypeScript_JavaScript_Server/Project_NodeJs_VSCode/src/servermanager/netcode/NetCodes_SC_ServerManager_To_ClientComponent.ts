import { NetCodeRanges } from "base/common/net/netcodes/NetCodeRanges";

export class NetCodes_SC_ServerManager_To_ClientComponent {

  // targetComponentTypeID: int (ComponentType.id)
  // ip: string
  // port: int
  public static readonly RESPONSE_SYNC_COMPONENT_LISTENER_IP_PORT_OK: number = NetCodeRanges.SC_ServerManagerToClientComponent + 1;

  public static readonly RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_OK: number = NetCodeRanges.SC_ServerManagerToClientComponent + 2;
  // targetComponentTypeID: int (ComponentType.id)
  public static readonly RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_COMPONENT_NOT_CONNECTED: number = NetCodeRanges.SC_ServerManagerToClientComponent + 3;

  // serverStatusId: int
  public static readonly REQUEST_SET_SERVER_STATUS: number = NetCodeRanges.SC_ServerManagerToClientComponent + 4;

}
