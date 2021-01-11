import { ILoopUserBase } from "./list/ILoopUserBase";

export class LoopDistributionBase {
    
    protected rootComponent: ILoopUserBase = null;
    
    public constructor() {
    //void
    }

    public getRootComponent(): ILoopUserBase {
        return this.rootComponent;
    }
    
    public setRootComponent(rootComponent: ILoopUserBase): void {
        this.rootComponent = rootComponent;
    }
    
    public distributeLoop(deltaS: number, deltaMS: number): void {
        //void
    }
    
}
    