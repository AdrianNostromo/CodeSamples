"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
const ComponentStatus_1 = require("base/common/util/ComponentStatus");
class CustomModule extends BasicAppComponent_1.BasicAppComponent {
    constructor(app, componentDependenciesList) {
        super(app);
        this.componentDependenciesList = componentDependenciesList;
    }
    componentStatusChanged(componentStatus) {
        super.componentStatusChanged(componentStatus);
        if (componentStatus == ComponentStatus_1.ComponentStatus.STARTING) {
            for (let i = 0; i < this.componentDependenciesList.length; i++) {
                let compDepCfg = this.componentDependenciesList[i];
                if (!this.getApp().getComponentDependenciesHandler().addDependency(compDepCfg.componentType, compDepCfg.componentStatusLayer)) {
                    this.getApp().getLog().error('Exit. Initialize error 1.');
                    this.getApp().getErrorHandler().error('Exit. Initialize error 1.');
                    return;
                }
            }
        }
    }
}
exports.CustomModule = CustomModule;
//# sourceMappingURL=CustomModule.js.map