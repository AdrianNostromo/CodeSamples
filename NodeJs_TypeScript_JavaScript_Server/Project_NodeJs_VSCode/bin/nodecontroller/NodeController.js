"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LocalCustomModule_1 = require("./module/LocalCustomModule");
const ComponentsProcessHandler_1 = require("nodecontroller/util/ComponentsProcessHandler");
class NodeController extends LocalCustomModule_1.LocalCustomModule {
    constructor(app) {
        super(app, [
        //void
        ]);
        this.componentsProcessHandler = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.componentsProcessHandler = new ComponentsProcessHandler_1.ComponentsProcessHandler(this.getApp()));
    }
}
exports.NodeController = NodeController;
//# sourceMappingURL=NodeController.js.map