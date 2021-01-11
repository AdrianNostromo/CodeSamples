import { IpPort } from "base/net/util/IpPort";

export class ClientListenerNetConfig {

    public ipPort: IpPort;

    public constructor(ipPort: IpPort) {
        this.ipPort = ipPort;
    }

}
