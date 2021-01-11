"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
class LocalAppComponent extends BasicAppComponent_1.BasicAppComponent {
    constructor(app) {
        super(app);
        this._app = app;
    }
    getApp() {
        return this._app;
    }
}
exports.LocalAppComponent = LocalAppComponent;
//# sourceMappingURL=LocalAppComponent.js.map