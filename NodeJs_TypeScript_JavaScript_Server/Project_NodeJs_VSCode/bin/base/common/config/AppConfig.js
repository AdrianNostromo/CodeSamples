"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const IpPort_1 = require("base/net/util/IpPort");
class AppConfig {
}
AppConfig.LOOP_FPS = 60;
// This is the number of physical pc nodes.
AppConfig.NODES_COUNT = 1;
AppConfig.DEFAULT_SERVER_MANAGER_IP_PORT = new IpPort_1.IpPort('127.0.0.1', 1201);
exports.AppConfig = AppConfig;
//# sourceMappingURL=AppConfig.js.map