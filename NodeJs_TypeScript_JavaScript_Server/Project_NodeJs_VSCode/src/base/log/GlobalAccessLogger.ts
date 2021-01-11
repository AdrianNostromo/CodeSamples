import { ILogger } from "./ILogger";

export class GlobalAccessLogger {
 
    private static log: ILogger = null;

    public static getLog(): ILogger {
        return GlobalAccessLogger.log;
    }

    public static setLog(log: ILogger): void {
        GlobalAccessLogger.log = log;
    }

}
