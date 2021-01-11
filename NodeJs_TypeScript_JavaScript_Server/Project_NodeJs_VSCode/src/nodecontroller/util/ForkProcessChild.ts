import { ComponentType } from "base/common/ComponentType";
import * as child_process from 'child_process';
import { IAppLocal } from "nodecontroller/app/IAppLocal";

export class ForkProcessChild {

  private app: IAppLocal;
  
  public _child: child_process.ChildProcess;
  
  public _componentType: ComponentType;
  public _componentTypeOnNodeIndex: number;

  public constructor(app: IAppLocal, child: child_process.ChildProcess, componentType: ComponentType, componentTypeOnNodeIndex: number) {
    this.app = app;
    
    this._child = child;
    
    this._componentType = componentType;
    this._componentTypeOnNodeIndex = componentTypeOnNodeIndex;
  }
  
  public getApp(): IAppLocal {
    return this.app;
  }
  
  public create(): void {
    this._child.stdout.on('data', this.childStdoutEvent.bind(this));
    
    this._child.on('error', this.childErrorEvent.bind(this));

    this._child.on('exit', this.childExitEvent.bind(this));
  }
  
  private childStdoutEvent(chunk: Buffer | string): void {
    this.getApp().getLog().info('Child(' + this._componentType.name + '_' + this._componentTypeOnNodeIndex + ' data: ' + chunk);
  }
  
  private childErrorEvent(err: Error): void {
    this.getApp().getLog().info('Child(' + this._componentType.name + '_' + this._componentTypeOnNodeIndex + ' error: ' + err);
  }
  
  private childExitEvent(code: number): void {
    // NOTE : exit code of 12 occurs also when the debug/inspect port manuall=y supplied is already in use.
    this.getApp().getLog().info('Child(' + this._componentType.name + '_' + this._componentTypeOnNodeIndex + ' exit: ' + code);
  }
  
}
