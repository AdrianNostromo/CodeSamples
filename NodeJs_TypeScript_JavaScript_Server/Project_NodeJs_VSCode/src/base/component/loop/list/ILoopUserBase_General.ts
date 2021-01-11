import { ILoopUserBase } from "./ILoopUserBase";

export interface ILoopUserBase_General extends ILoopUserBase {

    _ILoopUserBase_General: true;

    appLoop_General(deltaS: number, deltaMS: number): void;
  
}
