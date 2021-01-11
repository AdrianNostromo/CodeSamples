
export interface ILogger {

    getNextULogId(): number;
    
    debug(msg: string, msgId?: number, minDelayS?: number): void;
    
    info(msg: string, msgId?: number, minDelayS?: number): void;
    
    console(msg: string, msgId?: number, minDelayS?: number): void;
    
    warning(msg: string, msgId?: number, minDelayS?: number): void;
    
    error(msg: string, msgId?: number, minDelayS?: number): void;
    
    security(msg: string, msgId?: number, minDelayS?: number): void;
    
    securityWarning(msg: string, msgId?: number, minDelayS?: number): void;
    
    pushLog(prefix: string, msg: string, msgId?: number, minDelayS?: number): void;
    
}
