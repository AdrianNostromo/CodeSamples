import { BasicAppComponent } from "base/component/BasicAppComponent";

import { IApp } from "base/app/IApp";
import { ILoopUserBase_General } from "base/component/loop/list/ILoopUserBase_General";
import { ILogger } from "./ILogger";
import { GlobalAccessLogger } from "./GlobalAccessLogger";

export class Logger extends BasicAppComponent implements ILoopUserBase_General, ILogger {
  
  public _ILoopUserBase_General: true = true;
  
  private static uLogIdCounter: number = 0;

  private _delayMonitor: Map<number, number> = new Map();
  private _currentTimeS: number = 0;

  public constructor(app: IApp) {
    super(app);
    
    GlobalAccessLogger.setLog(this);
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    //void
  }
  
  public appLoop_General(deltaS: number, deltaMS: number): void {
    this._currentTimeS += deltaS;
  }
  
  public getNextULogId(): number {
    return ++Logger.uLogIdCounter;
  }
  
  public debug(msg: string, msgId?: number, minDelayS?: number): void {
    this.pushLog('d> ', msg, msgId, minDelayS);
  }
  
  public info(msg: string, msgId?: number, minDelayS?: number): void {
    this.pushLog('i> ', msg, msgId, minDelayS);
  }
  
  public console(msg: string, msgId?: number, minDelayS?: number): void {
    this.pushLog('c> ', msg, msgId, minDelayS);
  }
  
  public warning(msg: string, msgId?: number, minDelayS?: number): void {
    this.pushLog('w> ', msg, msgId, minDelayS);
  }
  
  public error(msg: string, msgId?: number, minDelayS?: number): void {
    this.pushLog('e> ', msg, msgId, minDelayS);
  }
  
  public security(msg: string, msgId?: number, minDelayS?: number): void {
    this.pushLog('s> ', msg, msgId, minDelayS);
  }
  
  public securityWarning(msg: string, msgId?: number, minDelayS?: number): void {
    this.pushLog('sw> ', msg, msgId, minDelayS);
  }
  
  public pushLog(prefix: string, msg: string, msgId?: number, minDelayS?: number): void {
    if(msgId !== undefined && msgId !== null && minDelayS !== undefined && minDelayS !== null) {
      if(!this._delayMonitor.has(msgId) || this._delayMonitor.get(msgId) + minDelayS <= this._currentTimeS) {
        // Post it;
        
        this._delayMonitor.set(msgId, this._currentTimeS);
      }else {
        return ;
      }
    }
    
    console.log(prefix + msg);
  }
  
}
