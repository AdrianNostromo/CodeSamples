export class ComponentStatus {

  private static entriesList: ComponentStatus[] = [];

  public static readonly OFFLINE: ComponentStatus = new ComponentStatus(
    1, 
    'OFFLINE', 
    false, 
    false
  );
  public static readonly STOPING: ComponentStatus = new ComponentStatus(
    2, 
    'STOPING', 
    true, 
    false
  );
  // Stating local modules.
  public static readonly STARTING: ComponentStatus = new ComponentStatus(
    3, 
    'STARTING', 
    true, 
    false
  );
  // Establishes connections to other server components.
  public static readonly CONNECTING: ComponentStatus = new ComponentStatus(
    4, 
    'CONNECTING', 
    true, 
    false
  );
  public static readonly ONLINE_FOR_COMPONENTS: ComponentStatus = new ComponentStatus(
    5, 
    'ONLINE_FOR_COMPONENTS', 
    true, 
    true
  );
  public static readonly ONLINE_FOR_WORLD: ComponentStatus = new ComponentStatus(
    6, 
    'ONLINE_FOR_WORLD', 
    false, 
    true
  );

  public id: number;
  public name: string;

  public isTransientState: boolean;
  public isOnline: boolean;

  public constructor(id: number, name: string, isTransientState: boolean, isOnline: boolean) {
    this.id = id;
    this.name = name;
    
    this.isTransientState = isTransientState;
    this.isOnline = isOnline;

    ComponentStatus.entriesList.push(this);
  }
  
  public static fromID(typeID: number): ComponentStatus {
    for(let i: number=0;i<ComponentStatus.entriesList.length;i++) {
      let entry: ComponentStatus = ComponentStatus.entriesList[i];
      if(entry != null && entry.id == typeID) {
        return entry;
      }
    }
    
    return null;
  }
  
  public static fromName(name: string): ComponentStatus {
    for(let i: number=0;i<ComponentStatus.entriesList.length;i++) {
      let entry: ComponentStatus = ComponentStatus.entriesList[i];
      if(entry != null && entry.name == name) {
        return entry;
      }
    }
    
    return null;
  }
  
}
