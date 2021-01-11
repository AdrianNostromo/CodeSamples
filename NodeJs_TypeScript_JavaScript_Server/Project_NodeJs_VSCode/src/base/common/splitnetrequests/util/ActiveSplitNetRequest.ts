export class ActiveSplitNetRequest {

  public splitNetID: number;
    
  public dataMap: Map<string, Object>;

  constructor(splitNetID: number, dataMap: Map<string, Object>) {
    this.splitNetID = splitNetID;
    
    this.dataMap = dataMap;
  }
  
}
