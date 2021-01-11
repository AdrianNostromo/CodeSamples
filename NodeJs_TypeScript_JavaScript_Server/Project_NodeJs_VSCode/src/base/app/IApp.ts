import { ILogger } from "base/log/ILogger";
import { IErrorHandler } from "base/error/IErrorHandler";
import { IConsoleHandler } from "base/console/IConsoleHandler";
import { IComponentDependenciesHandler } from "base/common/util/IComponentDependenciesHandler";
import { ComponentType } from "base/common/ComponentType";
import { ComponentStatus } from "base/common/util/ComponentStatus";
import { ComponentNetcutsHandler } from "base/common/net/ComponentNetcutsHandler";
import { ServerConnectionCallbacksHolder } from "base/common/util/ServerConnectionCallbacksHolder";
import { ServerStatus } from "base/common/server/ServerStatus";
import { IpPort } from "base/net/util/IpPort";
import { AppDirs } from "base/common/util/AppDirs";

export interface IApp {
    
    getComponentType(): ComponentType;
    
    getComponentStatus(): ComponentStatus;
    setComponentStatus(componentStatus: ComponentStatus): void;

    isStatus_OFFLINE(): boolean;
    isStatus_STOPING(): boolean;
    isStatus_STARTING(): boolean;
    isStatus_CONNECTING(): boolean;
    isStatus_ONLINE_FOR_COMPONENTS(): boolean;
    isStatus_ONLINE_FOR_WORLD(): boolean;
    isServerStatus_OFFLINE(): boolean;
    isServerStatus_ONLINE(): boolean;
    
    getServerStatus(): ServerStatus;
    setServerStatus(serverStatus: ServerStatus): void;
    
    getDirs(): AppDirs;
    
    getLog(): ILogger;
    getErrorHandler(): IErrorHandler;

    getConsoleHandler(): IConsoleHandler;
    getComponentDependenciesHandler(): IComponentDependenciesHandler;

    getClientComponentNetcuts(): ComponentNetcutsHandler;
    getServerComponentNetcuts(): ComponentNetcutsHandler;
    
    connectToServerComponent(componentType: ComponentType, ip: string, port: number, cbHolder: ServerConnectionCallbacksHolder): boolean;
    
    isListenerEnabled(): boolean;
    getListenerIpPort(): IpPort;
    
}
