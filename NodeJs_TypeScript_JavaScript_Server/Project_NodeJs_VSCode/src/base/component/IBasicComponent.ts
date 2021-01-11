
export interface IBasicComponent {

  reservedCreate(): void;
  
  dispose(): void;
  
  getParent<T extends IBasicComponent>(): T;
  setParent(parent: IBasicComponent): void;
  
  getRoot(): IBasicComponent;
  setRoot(root: IBasicComponent): void;
  
}
