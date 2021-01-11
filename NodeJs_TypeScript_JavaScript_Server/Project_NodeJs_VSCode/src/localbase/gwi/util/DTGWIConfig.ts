export class DTGWIConfig {

  public serverInstanceId: number;
  // eg: "Based". Must be the same as the client scenario config.
  public scenarioId: string;
  
  public maxPlayersCount: number;
  
  public constructor(
    serverInstanceId: number,
    scenarioId: string,

    maxPlayersCount: number
  ) {
    this.serverInstanceId = serverInstanceId;
    this.scenarioId = scenarioId;

    this.maxPlayersCount = maxPlayersCount;
  }
  
}
