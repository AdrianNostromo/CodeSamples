"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWINetListener_1 = require("./GWINetListener");
const RegisteredNetClientsSessionValidator_1 = require("squareheads/net/validator/RegisteredNetClientsSessionValidator");
class GWIClientsValidator extends GWINetListener_1.GWINetListener {
    constructor(handler, config) {
        super(handler, config);
        this.netClientsValidator = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this.addComponent(this.netClientsValidator = new RegisteredNetClientsSessionValidator_1.RegisteredNetClientsSessionValidator(this.onNetClientValidationSuccess.bind(this), this.handler.getClientDevicesManager(), this.handler.getClientSessionsManager()));
        this.netClientsValidator.name = "GameWorld_" + this.getServerInstanceId() + "_Validator";
    }
    onNewUnvalidatedCon(con) {
        this.netClientsValidator.addClientCon(con);
    }
}
exports.GWIClientsValidator = GWIClientsValidator;
//# sourceMappingURL=GWIClientsValidator.js.map