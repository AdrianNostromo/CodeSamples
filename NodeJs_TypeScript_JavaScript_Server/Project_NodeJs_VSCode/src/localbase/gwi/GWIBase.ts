import { BasicComponent } from "base/component/BasicComponent";
import { IBasicGWI } from "./IBasicGWI";
import { ILoopUserBase_GameWorld } from "base/component/loop/list/ILoopUserBase_GameWorld";
import { IHandler_GWI } from "./IHandler_GWI";
import { DTGWIConfig } from "./util/DTGWIConfig";
import { HandlerData } from "./util/HandlerData";
import { GlobalAccessLogger } from "base/log/GlobalAccessLogger";
import { IpPort } from "base/net/util/IpPort";

export abstract class GWIBase extends BasicComponent implements IBasicGWI, ILoopUserBase_GameWorld {
  
  public _ILoopUserBase_GameWorld: true = true;
  
  protected handler: IHandler_GWI;

  protected config: DTGWIConfig;
  
  protected isPlayEnabled: boolean = false;

  // Called when the match is no longer used.
  public gameWorldAutoRemoveTimerS: number = -1;

  // Managed by handler.
  protected handlerData: HandlerData = new HandlerData();

  private isInitFinished: boolean = false;

  public constructor(handler: IHandler_GWI, config: DTGWIConfig) {
    super();

    this.handler = handler;

    this.config = config;
  }

  public getIsInitFinished(): boolean {
    return this.isInitFinished;
  }

  public abstract getPlayerServerIpPort(): IpPort;
  public abstract isAnyPlayerConnected(): boolean;

  public getHandlerData(): HandlerData {
    return this.handlerData;
  }

  public getConfig(): DTGWIConfig {
    return this.config;
  }
  
  // extendable.
  protected checkIsInitFinished(): boolean {
    return true;
  }

  public getIsPlayEnabled(): boolean {
    return this.isPlayEnabled;
  }

  public setIsPlayEnabled(isPlayEnabled: boolean): void {
    this.isPlayEnabled = isPlayEnabled;
  }
  
  public appLoop_GameWorld(deltaS: number, deltaMS: number): void {
    if(!this.isInitFinished) {
      if(this.checkIsInitFinished()) {
        this.isInitFinished = true;
      }
    }

    if(this.isPlayEnabled) {
      this.gameLoop_netInputs(deltaS, deltaMS);
      this.gameLoop_simulation(deltaS, deltaMS);
      this.gameLoop_entitiesRemoval(deltaS, deltaMS);
      this.gameLoop_sync(deltaS, deltaMS);
    }
  }
  
  protected gameLoop_netInputs(deltaS: number, deltaMS: number): void {
    //void
  }
  
  protected gameLoop_simulation(deltaS: number, deltaMS: number): void {
    //void
  }
  
  protected gameLoop_entitiesRemoval(deltaS: number, deltaMS: number): void {
    //void
  }
  
  protected gameLoop_sync(deltaS: number, deltaMS: number): void {
    //void
  }
  
  public getServerInstanceId(): number {
    return this.config.serverInstanceId;
  }
  
  public getScenarioId(): string {
    return this.config.scenarioId;
  }
  
  protected netError(msg: string): void {
    GlobalAccessLogger.getLog().debug("GWIBase.netError.1: " + msg);
    
    if(this.handler) {
      this.handler.onNetError(this);
    }
  }
  
  protected logicError(msg: string): void {
    GlobalAccessLogger.getLog().debug("GWIBase.logicError.1: " + msg);

    if(this.handler) {
      this.handler.onLogicError(this);
    }
  }

  protected activateAutoRemoveTimerS(timeS: number): void {
    this.gameWorldAutoRemoveTimerS = timeS;
  }
  
  public dispose(): void {
    if(this.handler != null) {
      this.handler = null;
    }
    
    super.dispose();
  }
  
}
