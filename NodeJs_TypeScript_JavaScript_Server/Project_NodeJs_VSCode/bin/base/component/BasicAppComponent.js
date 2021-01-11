"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicComponent_1 = require("./BasicComponent");
class BasicAppComponent extends BasicComponent_1.BasicComponent {
    constructor(app) {
        super();
        this.app = app;
    }
    // This is used by the app component to set this as the app value.
    initApp(app) {
        this.app = app;
    }
    getApp() {
        return this.app;
    }
    componentStatusChanged(componentStatus) {
        let componentsList = this.getComponentsList();
        if (componentsList != null) {
            for (let i = 0; i < componentsList.length; i++) {
                let rawComp = componentsList[i];
                if (rawComp != null && rawComp instanceof BasicAppComponent) {
                    let comp = rawComp;
                    comp.componentStatusChanged(componentStatus);
                }
            }
        }
    }
    isComponentTransientStateFinished(componentStatus) {
        let componentsList = this.getComponentsList();
        if (componentsList != null) {
            for (let i = 0; i < componentsList.length; i++) {
                let rawComp = componentsList[i];
                if (rawComp != null && rawComp instanceof BasicAppComponent) {
                    let comp = rawComp;
                    let ret = comp.isComponentTransientStateFinished(componentStatus);
                    if (ret === false) {
                        return false;
                    }
                    else if (ret === true) {
                        //void
                    }
                    else {
                        throw new Error("Error.");
                    }
                }
            }
        }
        return true;
    }
}
exports.BasicAppComponent = BasicAppComponent;
//# sourceMappingURL=BasicAppComponent.js.map