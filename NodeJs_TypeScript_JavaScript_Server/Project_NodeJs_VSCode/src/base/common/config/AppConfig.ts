import { IpPort } from "base/net/util/IpPort";

export class AppConfig {
  
  public static readonly LOOP_FPS: number = 60;

  // This is the number of physical pc nodes.
  public static readonly NODES_COUNT: number = 1;

  public static readonly DEFAULT_SERVER_MANAGER_IP_PORT: IpPort = new IpPort(
    '127.0.0.1', 
    1201
  );

}
