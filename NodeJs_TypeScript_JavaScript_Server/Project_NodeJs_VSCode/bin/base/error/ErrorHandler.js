"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
class ErrorHandler extends BasicAppComponent_1.BasicAppComponent {
    constructor(app) {
        super(app);
        //void
    }
    create_framework() {
        super.create_framework();
        //void
    }
    error(msg) {
        console.log('Exit. ' + this.getApp().getComponentType().name + ": " + 'Critical error: ' + msg);
        process.exit(-2);
    }
}
exports.ErrorHandler = ErrorHandler;
//# sourceMappingURL=ErrorHandler.js.map