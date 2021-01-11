import { IBasicComponent } from "./IBasicComponent";

import { LogicError } from "base/exception/LogicError";

export class BasicTreeComponent implements IBasicComponent {
  
  private componentsList: IBasicComponent[] = [];
  
  private isCreated: boolean = false;
  private isDisposed: boolean = false;
  
  private root: IBasicComponent = null;
  private parent: IBasicComponent = null;
  
  public constructor() {
    //void
  }
    
  protected getComponentsList(): IBasicComponent[] {
    return this.componentsList;
  }
  
  public getRoot(): IBasicComponent {
      return this.root;
  }
  
  public setRoot(root: IBasicComponent): void {
      this.root = root;
  }
  
  public getParent<T extends IBasicComponent>(): T {
      return <T>this.parent;
  }
  
  public setParent(parent: IBasicComponent): void {
      this.parent = parent;
  }
  
  public reservedCreate(): void {
    if(this.isCreated) {
      throw new LogicError("BaseAppComponent.x_dispose.1.");
    }
    this.isCreated = true;
    
    this.create_framework();
    this.create_content();
    this.create_post();
  }

  protected create_framework(): void {
    //void
  }
  
  protected create_content(): void {
    //void
  }
  
  protected create_post(): void {
    //void
  }
  
  protected addComponent(component: IBasicComponent): void {
    if(component == null || this.componentsList.indexOf(component) >= 0) {
      throw new Error("Error. 1.");
    }

    if(this.root == null) {
      // This component must be on the components tree.
      throw new Error("Error. 1.");
    }
    
    if(component.getRoot() != null) {
      // Child component must not be on the components tree.
      throw new Error("Error. 1.");
    }
    
    component.setRoot(this.root);
    component.setParent(this);

    this.componentsList.push(component);

    component.reservedCreate();

    this.onSubcomponentAdded(component);
  }

  public onSubcomponentAdded(component: IBasicComponent): void {
    //void
  }

  public onSubcomponentRemoving_pre(component: IBasicComponent): void {
    //void
  }

  protected disposeComponent(component: IBasicComponent): void {
    if(this.root == null) {
      throw new Error("Error. 1.");
    }
    // "root" doesn't have a "parent".
    if(this.parent == null && this != this.root) {
        throw new Error("Error. 1.");
    }
    
    if(component.getRoot() == null) {
        throw new Error("Error. 1.");
    }
    if(component.getParent() != this) {
        throw new Error("Error. 1.");
    }

    this.onSubcomponentRemoving_pre(component);

    let index: number = this.componentsList.indexOf(component);
    if(index < 0) {
      throw new Error("Error.");
    }

    component.dispose();
    
    component.setRoot(null);
    component.setParent(null);

    this.componentsList.splice(index, 1);
  }
  
  public getIsDisposed(): boolean {
      return this.isDisposed;
  }
  
  public dispose(): void {
    if(this.isDisposed) {
        throw new LogicError("BaseAppComponent.dispose.1.");
    }
    this.isDisposed = true;
    
    //void
  }
  
}
