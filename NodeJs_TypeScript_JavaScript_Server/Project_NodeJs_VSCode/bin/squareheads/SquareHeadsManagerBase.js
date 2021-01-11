"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const EntryPointInteractor_1 = require("./entrypointinteractor/EntryPointInteractor");
const LocalCustomModule_1 = require("./module/LocalCustomModule");
class SquareHeadsManagerBase extends LocalCustomModule_1.LocalCustomModule {
    constructor(app) {
        super(app, [
        //void
        ]);
        this.entryPointInteractor = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.entryPointInteractor = new EntryPointInteractor_1.EntryPointInteractor(this.getApp()));
    }
}
exports.SquareHeadsManagerBase = SquareHeadsManagerBase;
//# sourceMappingURL=SquareHeadsManagerBase.js.map