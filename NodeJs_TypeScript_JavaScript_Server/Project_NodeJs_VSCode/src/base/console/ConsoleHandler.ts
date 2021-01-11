import { BasicAppComponent } from "base/component/BasicAppComponent";
import { IApp } from "base/app/IApp";
import * as readline from "readline";
import { ComponentStatus } from "base/common/util/ComponentStatus";
import { IConsoleHandler } from "./IConsoleHandler";

export class ConsoleHandler extends BasicAppComponent implements IConsoleHandler {

    private consoleInput: readline.ReadLine;

    public constructor(app: IApp) {
        super(app);
        
        //void
    }
  
    protected create_framework(): void {
        super.create_framework();

        this.consoleInput = readline.createInterface({
            input: process.stdin,
            output: process.stdout
        });
            
        this.consoleInput.question("Console input enabled. Awaiting command.\n", this.onConsoleInput.bind(this));
    }

    private onConsoleInput(answer: string): void {
        let commandResult: string = null;

        if(answer == "shutdown") {
            if(this.getApp().getComponentStatus() == ComponentStatus.ONLINE_FOR_COMPONENTS ||
                this.getApp().getComponentStatus() == ComponentStatus.ONLINE_FOR_WORLD)
            {
                this.getApp().setComponentStatus(ComponentStatus.STOPING);

                commandResult = "Shutdown started.";
            }else {
                commandResult = "Component must be online to do shutdown.";
            }
        }else {
            commandResult = "Bad command.";
        }

        this.consoleInput.question(commandResult + " Console input enabled. Awaiting command.\n", this.onConsoleInput.bind(this));
    }
  
}
