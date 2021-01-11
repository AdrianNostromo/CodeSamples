"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const NetCodeRanges_1 = require("base/common/net/netcodes/NetCodeRanges");
class NetCodes_SC_ServerManager_To_ClientComponent {
}
// targetComponentTypeID: int (ComponentType.id)
// ip: string
// port: int
NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_SYNC_COMPONENT_LISTENER_IP_PORT_OK = NetCodeRanges_1.NetCodeRanges.SC_ServerManagerToClientComponent + 1;
NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_OK = NetCodeRanges_1.NetCodeRanges.SC_ServerManagerToClientComponent + 2;
// targetComponentTypeID: int (ComponentType.id)
NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_COMPONENT_NOT_CONNECTED = NetCodeRanges_1.NetCodeRanges.SC_ServerManagerToClientComponent + 3;
// serverStatusId: int
NetCodes_SC_ServerManager_To_ClientComponent.REQUEST_SET_SERVER_STATUS = NetCodeRanges_1.NetCodeRanges.SC_ServerManagerToClientComponent + 4;
exports.NetCodes_SC_ServerManager_To_ClientComponent = NetCodes_SC_ServerManager_To_ClientComponent;
//# sourceMappingURL=NetCodes_SC_ServerManager_To_ClientComponent.js.map