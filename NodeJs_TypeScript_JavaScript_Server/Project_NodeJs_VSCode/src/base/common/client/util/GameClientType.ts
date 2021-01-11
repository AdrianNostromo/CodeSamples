export class GameClientType {

  private static entriesList: GameClientType[] = [];

  public static readonly SQUARE_HEADS: GameClientType = new GameClientType(
    1, 
    'SQUARE_HEADS'
  );

  public id: number;
  public name: string;
  
  public constructor(id: number, name: string) {
    this.id = id;
    this.name = name;

    GameClientType.entriesList.push(this);
  }
  
  public static fromID(typeID: number): GameClientType {
    for(let i: number=0;i<GameClientType.entriesList.length;i++) {
      let entry: GameClientType = GameClientType.entriesList[i];
      if(entry != null && entry.id == typeID) {
        return entry;
      }
    }
    
    return null;
  }
  
  public static fromName(name: string): GameClientType {
    for(let i: number=0;i<GameClientType.entriesList.length;i++) {
      let entry: GameClientType = GameClientType.entriesList[i];
      if(entry != null && entry.name == name) {
        return entry;
      }
    }
    
    return null;
  }
  
}
