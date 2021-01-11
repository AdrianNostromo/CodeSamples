"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
const readline = require("readline");
const ComponentStatus_1 = require("base/common/util/ComponentStatus");
class ConsoleHandler extends BasicAppComponent_1.BasicAppComponent {
    constructor(app) {
        super(app);
        //void
    }
    create_framework() {
        super.create_framework();
        this.consoleInput = readline.createInterface({
            input: process.stdin,
            output: process.stdout
        });
        this.consoleInput.question("Console input enabled. Awaiting command.\n", this.onConsoleInput.bind(this));
    }
    onConsoleInput(answer) {
        let commandResult = null;
        if (answer == "shutdown") {
            if (this.getApp().getComponentStatus() == ComponentStatus_1.ComponentStatus.ONLINE_FOR_COMPONENTS ||
                this.getApp().getComponentStatus() == ComponentStatus_1.ComponentStatus.ONLINE_FOR_WORLD) {
                this.getApp().setComponentStatus(ComponentStatus_1.ComponentStatus.STOPING);
                commandResult = "Shutdown started.";
            }
            else {
                commandResult = "Component must be online to do shutdown.";
            }
        }
        else {
            commandResult = "Bad command.";
        }
        this.consoleInput.question(commandResult + " Console input enabled. Awaiting command.\n", this.onConsoleInput.bind(this));
    }
}
exports.ConsoleHandler = ConsoleHandler;
//# sourceMappingURL=ConsoleHandler.js.map