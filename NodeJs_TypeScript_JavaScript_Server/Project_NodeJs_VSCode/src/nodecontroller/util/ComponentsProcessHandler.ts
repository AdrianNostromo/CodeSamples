import { IAppLocal } from "./../app/IAppLocal";

import * as child_process from 'child_process';
import { ComponentType } from "base/common/ComponentType";
import { ForkProcessChild } from "./ForkProcessChild";
import { LocalAppComponent } from "nodecontroller/component/LocalAppComponent";

class ForkOptions implements child_process.ForkOptions {
  
  public cwd: string;
  public env: any;
  public execPath: string;
  public execArgv: string[];
  public silent: boolean;
  public stdio: any[];
  public uid: number;
  public gid: number;

  public constructor() {
    //void
  }

}

export class ComponentsProcessHandler extends LocalAppComponent {

  private _componentProcessesList: ForkProcessChild[] = [];
  private _componentTypeOnNodeIndexCounter: Map<number, number> = new Map();

  // asd_tst_001;// on release, remove the component from the debug list.
  private _debugInspectorPortCounter: number = 3070;
  private _targetComponentTypeToDebugList: ComponentType[] = [
    ComponentType.SERVER_MANAGER,
    
    ComponentType.NODE_CONTROLLER,
    
    ComponentType.LOG_MANAGER
  ];

  public constructor(app: IAppLocal) {
    super(app);
    
    //voi
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    //void
  }
  
  // Return: true: ok, false: error;
  public createNewComponent(componentType: ComponentType): boolean {
    if(!this._componentTypeOnNodeIndexCounter.has(componentType.id)) {
      this._componentTypeOnNodeIndexCounter.set(componentType.id, 0);
    }
    
    let componentTypeOnNodeIndex: number = this._componentTypeOnNodeIndexCounter.get(componentType.id) + 1;
    this._componentTypeOnNodeIndexCounter.set(componentType.id, componentTypeOnNodeIndex);

    let s: string = 'Creating component fork. Component type: ' + componentType.name + '. Component type on node index: ' + componentTypeOnNodeIndex + '.';
    
    let opts: child_process.ForkOptions = new ForkOptions();
    opts.stdio = [
      'pipe', // stdin
      'pipe', // stdout
      'pipe', // stderr
      'ipc'
    ];
    
    if(this._targetComponentTypeToDebugList.indexOf(componentType) >= 0) {
      this._debugInspectorPortCounter++;
      
      // Recommended to not use debug/inspect args. Manually start the module to debug it.
      opts.execArgv = [
        /*'--inspect'/*,
        /*'--inspect=' + this._debugInspectorPortCounter/*,
        '--debug',
        '--debug-brk'*/
      ];
      
      s += 'Debug inspector port: ' + this._debugInspectorPortCounter + '.';
    }else {
      opts.execArgv = [];
    }
    
    this.getApp().getLog().info(s);
    
    let args: string[] = [
      componentType.name, 
      String(componentTypeOnNodeIndex)
    ];

    let child:child_process.ChildProcess = child_process.fork('./bin/main', args, opts);
    
    let forkObject: ForkProcessChild = new ForkProcessChild(this.getApp(), child, componentType, componentTypeOnNodeIndex);
    forkObject.create();
    
    this._componentProcessesList.push(forkObject);
    
    return true;
  }
  
}
