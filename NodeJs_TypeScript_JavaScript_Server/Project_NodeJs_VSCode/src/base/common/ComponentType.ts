export class ComponentType {

  private static entriesList: ComponentType[] = [];

  public static readonly SERVER_MANAGER: ComponentType = new ComponentType(
    1, 
    'SERVER_MANAGER'
  );
  public static readonly NODE_CONTROLLER: ComponentType = new ComponentType(
    2, 
    'NODE_CONTROLLER'
  );
  public static readonly LOG_MANAGER: ComponentType = new ComponentType(
    3, 
    'LOG_MANAGER'
  );

  public static readonly SQUARE_HEADS_MANAGER: ComponentType = new ComponentType(
    5, 
    'SQUARE_HEADS_MANAGER'
  );
  
  public id: number;
  public name: string;

  public constructor(id: number, name: string) {
    this.id = id;
    this.name = name;

    ComponentType.entriesList.push(this);
  }
  
  public static fromID(typeID: number): ComponentType {
    for(let i: number=0;i<ComponentType.entriesList.length;i++) {
      let entry: ComponentType = ComponentType.entriesList[i];
      if(entry != null && entry.id == typeID) {
        return entry;
      }
    }
    
    return null;
  }
  
  public static fromName(name: string): ComponentType {
    for(let i: number=0;i<ComponentType.entriesList.length;i++) {
      let entry: ComponentType = ComponentType.entriesList[i];
      if(entry != null && entry.name == name) {
        return entry;
      }
    }
    
    return null;
  }
  
}
