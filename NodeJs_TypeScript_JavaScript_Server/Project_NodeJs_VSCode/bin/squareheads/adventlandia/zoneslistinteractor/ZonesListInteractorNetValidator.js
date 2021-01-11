"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ZonesListInteractorNetListener_1 = require("./ZonesListInteractorNetListener");
const RegisteredNetClientsSessionValidator_1 = require("squareheads/net/validator/RegisteredNetClientsSessionValidator");
class ZonesListInteractorNetValidator extends ZonesListInteractorNetListener_1.ZonesListInteractorNetListener {
    constructor(app) {
        super(app);
        this.netClientsValidator = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.netClientsValidator = new RegisteredNetClientsSessionValidator_1.RegisteredNetClientsSessionValidator(this.onNetClientValidationSuccess.bind(this), this.getApp().getSquareHeads().getClientDevicesManager(), this.getApp().getSquareHeads().getClientSessionsManager()));
    }
    onNewUnvalidatedCon(con) {
        this.netClientsValidator.addClientCon(con);
    }
}
exports.ZonesListInteractorNetValidator = ZonesListInteractorNetValidator;
//# sourceMappingURL=ZonesListInteractorNetValidator.js.map