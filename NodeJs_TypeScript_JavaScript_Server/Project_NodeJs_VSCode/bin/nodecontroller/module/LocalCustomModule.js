"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const CustomModule_1 = require("base/module/CustomModule");
class LocalCustomModule extends CustomModule_1.CustomModule {
    constructor(app, componentDependenciesList) {
        super(app, componentDependenciesList);
        this._app = app;
    }
    getApp() {
        return this._app;
    }
}
exports.LocalCustomModule = LocalCustomModule;
//# sourceMappingURL=LocalCustomModule.js.map