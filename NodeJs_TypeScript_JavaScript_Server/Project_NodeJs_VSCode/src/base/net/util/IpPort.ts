export class IpPort {

  public ip: string;
  public port: number;

  public constructor(ip: string, port: number) {
    this.set(ip, port);
  }
  
  public set(ip: string, port: number): void {
    this.ip = ip;
    this.port = port;
  }

}
