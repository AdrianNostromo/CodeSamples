"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const child_process = require("child_process");
const ComponentType_1 = require("base/common/ComponentType");
const ForkProcessChild_1 = require("./ForkProcessChild");
const LocalAppComponent_1 = require("nodecontroller/component/LocalAppComponent");
class ForkOptions {
    constructor() {
        //void
    }
}
class ComponentsProcessHandler extends LocalAppComponent_1.LocalAppComponent {
    constructor(app) {
        super(app);
        this._componentProcessesList = [];
        this._componentTypeOnNodeIndexCounter = new Map();
        // asd_tst_001;// on release, remove the component from the debug list.
        this._debugInspectorPortCounter = 3070;
        this._targetComponentTypeToDebugList = [
            ComponentType_1.ComponentType.SERVER_MANAGER,
            ComponentType_1.ComponentType.NODE_CONTROLLER,
            ComponentType_1.ComponentType.LOG_MANAGER
        ];
        //voi
    }
    create_framework() {
        super.create_framework();
        //void
    }
    // Return: true: ok, false: error;
    createNewComponent(componentType) {
        if (!this._componentTypeOnNodeIndexCounter.has(componentType.id)) {
            this._componentTypeOnNodeIndexCounter.set(componentType.id, 0);
        }
        let componentTypeOnNodeIndex = this._componentTypeOnNodeIndexCounter.get(componentType.id) + 1;
        this._componentTypeOnNodeIndexCounter.set(componentType.id, componentTypeOnNodeIndex);
        let s = 'Creating component fork. Component type: ' + componentType.name + '. Component type on node index: ' + componentTypeOnNodeIndex + '.';
        let opts = new ForkOptions();
        opts.stdio = [
            'pipe',
            'pipe',
            'pipe',
            'ipc'
        ];
        if (this._targetComponentTypeToDebugList.indexOf(componentType) >= 0) {
            this._debugInspectorPortCounter++;
            // Recommended to not use debug/inspect args. Manually start the module to debug it.
            opts.execArgv = [
            /*'--inspect'/*,
            /*'--inspect=' + this._debugInspectorPortCounter/*,
            '--debug',
            '--debug-brk'*/
            ];
            s += 'Debug inspector port: ' + this._debugInspectorPortCounter + '.';
        }
        else {
            opts.execArgv = [];
        }
        this.getApp().getLog().info(s);
        let args = [
            componentType.name,
            String(componentTypeOnNodeIndex)
        ];
        let child = child_process.fork('./bin/main', args, opts);
        let forkObject = new ForkProcessChild_1.ForkProcessChild(this.getApp(), child, componentType, componentTypeOnNodeIndex);
        forkObject.create();
        this._componentProcessesList.push(forkObject);
        return true;
    }
}
exports.ComponentsProcessHandler = ComponentsProcessHandler;
//# sourceMappingURL=ComponentsProcessHandler.js.map