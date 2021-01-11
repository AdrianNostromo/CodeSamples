import { BasicTreeComponent } from "./BasicTreeComponent";

import { ILoopUserBase } from "./loop/list/ILoopUserBase";
import { LoopListTypes } from "./loop/config/LoopListTypes";
import { InterfaceUtil } from "base/util/InterfaceUtil";
import { LogicError } from "base/exception/LogicError";

export class BasicLoopComponent extends BasicTreeComponent implements ILoopUserBase {

  // Looping chained list.
  private readonly prevLoopUserNodesList: ILoopUserBase[] = [];
  // Looping chained list.
  private readonly nextLoopUserNodesList: ILoopUserBase[] = [];
  
  // Contains a loop user if one is active.
  // It can be this.
  // If this is not a loop user, it can be a sub component that uses the specific loop.
  private readonly floatingLoopUsersList: ILoopUserBase[] = [];

  public constructor() {
      super();
      
      //void
  }
  
  public getPrevLoopUserNode<T extends ILoopUserBase>(loopIndex: number): T {
      return <T>this.prevLoopUserNodesList[loopIndex];
  }
  
  public setPrevLoopUserNode(loopIndex: number, loopUserNode: ILoopUserBase): ILoopUserBase {
      return (this.prevLoopUserNodesList[loopIndex] = loopUserNode);
  }
  
  public insertPrevLoopUserNode(loopIndex: number, loopUserNode: ILoopUserBase): ILoopUserBase {
      loopUserNode.setPrevLoopUserNode(loopIndex, this.getPrevLoopUserNode(loopIndex));
      this.getPrevLoopUserNode(loopIndex).setNextLoopUserNode(loopIndex, loopUserNode);
  
      loopUserNode.setNextLoopUserNode(loopIndex, this);
      this.setPrevLoopUserNode(loopIndex, loopUserNode);
      
      return loopUserNode;
  }
  
  public getNextLoopUserNode<T extends ILoopUserBase>(loopIndex: number): T {
      return <T>this.nextLoopUserNodesList[loopIndex];
  }
  
  public setNextLoopUserNode(loopIndex: number, loopUserNode: ILoopUserBase): ILoopUserBase {
      return (this.nextLoopUserNodesList[loopIndex] = loopUserNode);
  }
  
  public insertNextLoopUserNode(loopIndex: number, loopUserNode: ILoopUserBase): ILoopUserBase {
      loopUserNode.setNextLoopUserNode(loopIndex, this.getNextLoopUserNode(loopIndex));
      this.getNextLoopUserNode(loopIndex).setPrevLoopUserNode(loopIndex, loopUserNode);
  
      loopUserNode.setPrevLoopUserNode(loopIndex, this);
      this.setNextLoopUserNode(loopIndex, loopUserNode);
  
      return loopUserNode;
  }
  
  public removeLoopUserNode(loopIndex: number): void {
      this.getPrevLoopUserNode(loopIndex).setNextLoopUserNode(loopIndex, this.getNextLoopUserNode(loopIndex));
      this.getNextLoopUserNode(loopIndex).setPrevLoopUserNode(loopIndex, this.getPrevLoopUserNode(loopIndex));
      
      this.setPrevLoopUserNode(loopIndex, this);
      this.setNextLoopUserNode(loopIndex, this);
  }
  
  public descendantLoopUserAdded(loopIndex: number, component: ILoopUserBase): void {
      if(this.floatingLoopUsersList[loopIndex] == null) {
          this.floatingLoopUsersList[loopIndex] = component;
          
          if(this != this.getRoot()) {
              this.getParent<ILoopUserBase>().descendantLoopUserAdded(loopIndex, component);
          }
      }else {
        if(component == this && this.floatingLoopUsersList[loopIndex] != this) {
            // A child component hijacked the value  before "this" was able to set it.
            // this.floatingLoopUsersList[loopIndex] != this
            throw new LogicError("Error. 1.");
          }
        
          if(this.floatingLoopUsersList[loopIndex] == this) {
              // "this" is the floating loop user.
              // Add the descendant loop user as a prev.
              this.floatingLoopUsersList[loopIndex].insertPrevLoopUserNode(loopIndex, component);
          }else {
              // The current float loop user is a descendant of "this".
              // Add the descendant loop user as a next.
              this.floatingLoopUsersList[loopIndex].insertNextLoopUserNode(loopIndex, component);
              
              // Also save the descendant as the floating loop user.
              this.floatingLoopUsersList[loopIndex] = component;
          }
      }
  }
  
  public descendantLoopUserRemovingPre(loopIndex: number, component: ILoopUserBase): void {
      // Remove the component from "floatingLoopUsersList" if it is in the list.
      // The "floatingLoopUsersList" list may contain a descendant if "this" doesn't use a loop.
      // If in the list, also call this method on the parent.
      if(this.floatingLoopUsersList[loopIndex] == component) {
          this.floatingLoopUsersList[loopIndex] = null;
          
          if(this != this.getRoot()) {
            this.getParent<ILoopUserBase>().descendantLoopUserRemovingPre(loopIndex, component);
          }
      }
  }
  
  public getFloatingLoopUser(loopIndex: number): ILoopUserBase {
      return this.floatingLoopUsersList[loopIndex];
  }
  
  protected create_framework(): void {
    super.create_framework();

    if(InterfaceUtil.implementsInterface(this, "ILoopUserBase_General")) {
      this.prevLoopUserNodesList[LoopListTypes.LOOP_INDEX_GENERAL] = this;
      this.nextLoopUserNodesList[LoopListTypes.LOOP_INDEX_GENERAL] = this;
    }
    if(InterfaceUtil.implementsInterface(this, "ILoopUserBase_Eco")) {
        this.prevLoopUserNodesList[LoopListTypes.LOOP_INDEX_ECO] = this;
        this.nextLoopUserNodesList[LoopListTypes.LOOP_INDEX_ECO] = this;
    }
    if(InterfaceUtil.implementsInterface(this, "ILoopUserBase_GameWorld")) {
        this.prevLoopUserNodesList[LoopListTypes.LOOP_INDEX_GAME_WORLD] = this;
        this.nextLoopUserNodesList[LoopListTypes.LOOP_INDEX_GAME_WORLD] = this;
    }
    
    if(InterfaceUtil.implementsInterface(this, "ILoopUserBase_General")) {
      this.descendantLoopUserAdded(LoopListTypes.LOOP_INDEX_GENERAL, <ILoopUserBase><Object>this);
    }
    if(InterfaceUtil.implementsInterface(this, "ILoopUserBase_Eco")) {
        this.descendantLoopUserAdded(LoopListTypes.LOOP_INDEX_ECO, <ILoopUserBase><Object>this);
    }
    if(InterfaceUtil.implementsInterface(this, "ILoopUserBase_GameWorld")) {
        this.descendantLoopUserAdded(LoopListTypes.LOOP_INDEX_GAME_WORLD, <ILoopUserBase><Object>this);
    }
  }
    
  public dispose(): void {
    if(InterfaceUtil.implementsInterface(this, "ILoopUserBase_General")) {
      this.descendantLoopUserRemovingPre(LoopListTypes.LOOP_INDEX_GENERAL, <ILoopUserBase><Object>this);
    }
    if(InterfaceUtil.implementsInterface(this, "ILoopUserBase_Eco")) {
        this.descendantLoopUserRemovingPre(LoopListTypes.LOOP_INDEX_ECO, <ILoopUserBase><Object>this);
    }
    if(InterfaceUtil.implementsInterface(this, "ILoopUserBase_GameWorld")) {
        this.descendantLoopUserRemovingPre(LoopListTypes.LOOP_INDEX_GAME_WORLD, <ILoopUserBase><Object>this);
    }
    
    super.dispose();
  }
  
}
