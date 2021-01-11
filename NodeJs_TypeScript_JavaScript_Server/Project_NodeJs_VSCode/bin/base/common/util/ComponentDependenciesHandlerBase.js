"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
class ComponentDependenciesHandlerBase extends BasicAppComponent_1.BasicAppComponent {
    constructor(app) {
        super(app);
        // These dependencies are cleared every time they are required. Except for the server Manager.
        // .componentTypeID: {ip: string, port: int};
        this._dependencyComponentListenersIpPort = new Map();
        //void
    }
    addDependencyComponentListenerIpPort(componentType, ipPort) {
        if (this._dependencyComponentListenersIpPort.get(componentType.id)) {
            return false;
        }
        this._dependencyComponentListenersIpPort.set(componentType.id, ipPort);
        return true;
    }
    clearDependencyComponentListenerIpPort(componentType) {
        this._dependencyComponentListenersIpPort.delete(componentType.id);
    }
    getDependencyComponentListenerIpPort(componentType) {
        let ret = this._dependencyComponentListenersIpPort.get(componentType.id);
        return ret;
    }
}
exports.ComponentDependenciesHandlerBase = ComponentDependenciesHandlerBase;
//# sourceMappingURL=ComponentDependenciesHandlerBase.js.map