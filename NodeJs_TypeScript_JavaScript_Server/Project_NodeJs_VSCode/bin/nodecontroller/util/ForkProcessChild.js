"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class ForkProcessChild {
    constructor(app, child, componentType, componentTypeOnNodeIndex) {
        this.app = app;
        this._child = child;
        this._componentType = componentType;
        this._componentTypeOnNodeIndex = componentTypeOnNodeIndex;
    }
    getApp() {
        return this.app;
    }
    create() {
        this._child.stdout.on('data', this.childStdoutEvent.bind(this));
        this._child.on('error', this.childErrorEvent.bind(this));
        this._child.on('exit', this.childExitEvent.bind(this));
    }
    childStdoutEvent(chunk) {
        this.getApp().getLog().info('Child(' + this._componentType.name + '_' + this._componentTypeOnNodeIndex + ' data: ' + chunk);
    }
    childErrorEvent(err) {
        this.getApp().getLog().info('Child(' + this._componentType.name + '_' + this._componentTypeOnNodeIndex + ' error: ' + err);
    }
    childExitEvent(code) {
        // NOTE : exit code of 12 occurs also when the debug/inspect port manuall=y supplied is already in use.
        this.getApp().getLog().info('Child(' + this._componentType.name + '_' + this._componentTypeOnNodeIndex + ' exit: ' + code);
    }
}
exports.ForkProcessChild = ForkProcessChild;
//# sourceMappingURL=ForkProcessChild.js.map