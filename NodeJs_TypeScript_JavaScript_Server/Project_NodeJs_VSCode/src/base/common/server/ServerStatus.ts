export class ServerStatus {

  private static entriesList: ServerStatus[] = [];

  public static readonly OFFLINE: ServerStatus = new ServerStatus(
    1, 
    'OFFLINE', 
    false
  );
  public static readonly ONLINE: ServerStatus = new ServerStatus(
    4, 
    'ONLINE', 
    false
  );

  public id: number;
  public name: string;

  public isTransientState: boolean;

  constructor(id: number, name: string, isTransientState: boolean) {
    this.id = id;
    this.name = name;

    this.isTransientState = isTransientState;

    ServerStatus.entriesList.push(this);
  }
  
  public static fromID(typeID: number): ServerStatus {
    for(let i: number=0;i<ServerStatus.entriesList.length;i++) {
      let entry: ServerStatus = ServerStatus.entriesList[i];
      if(entry != null && entry.id == typeID) {
        return entry;
      }
    }
    
    return null;
  }
  
  public static fromName(name: string): ServerStatus {
    for(let i: number=0;i<ServerStatus.entriesList.length;i++) {
      let entry: ServerStatus = ServerStatus.entriesList[i];
      if(entry != null && entry.name == name) {
        return entry;
      }
    }
    
    return null;
  }
  
}
