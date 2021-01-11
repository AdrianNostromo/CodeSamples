import { ILoopUserBase } from "./ILoopUserBase";

export interface ILoopUserBase_GameWorld extends ILoopUserBase {

    _ILoopUserBase_GameWorld: true;

    appLoop_GameWorld(deltaS: number, deltaMS: number): void;
  
}
