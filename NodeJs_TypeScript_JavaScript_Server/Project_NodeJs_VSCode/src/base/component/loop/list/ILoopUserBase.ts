import { IBasicComponent } from "base/component/IBasicComponent";

export interface ILoopUserBase extends IBasicComponent {

  getPrevLoopUserNode<T extends ILoopUserBase>(loopIndex: number): T;
  setPrevLoopUserNode(loopIndex: number, prevLoopUser: ILoopUserBase): ILoopUserBase;
  insertPrevLoopUserNode(loopIndex: number, prevLoopUser: ILoopUserBase): ILoopUserBase;
  
  getNextLoopUserNode<T extends ILoopUserBase>(loopIndex: number): T;
  setNextLoopUserNode(loopIndex: number, nextLoopUser: ILoopUserBase): ILoopUserBase;
  insertNextLoopUserNode(loopIndex: number, nextLoopUser: ILoopUserBase): ILoopUserBase;
  
  removeLoopUserNode(loopIndex: number): void;
  
  descendantLoopUserAdded(loopIndex: number, component: ILoopUserBase): void;
  descendantLoopUserRemovingPre(loopIndex: number, component: ILoopUserBase): void;
  
  getIsDisposed(): boolean;
  
  getFloatingLoopUser(loopIndex: number): ILoopUserBase;
  
}
