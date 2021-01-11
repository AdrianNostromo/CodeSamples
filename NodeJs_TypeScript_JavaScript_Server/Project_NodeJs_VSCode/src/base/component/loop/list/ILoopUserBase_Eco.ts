import { ILoopUserBase } from "./ILoopUserBase";

export interface ILoopUserBase_Eco extends ILoopUserBase {

    _ILoopUserBase_Eco: true;

    appLoop_Eco(deltaS: number, deltaMS: number): void;
  
}
