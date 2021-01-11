import { LoopDistributionBase } from "./LoopDistributionBase";

import { ILoopUserBase_General } from "./list/ILoopUserBase_General";
import { LoopListTypes } from "./config/LoopListTypes";

export class LoopDistributionGeneral extends LoopDistributionBase {

    // Must be float because it is used in float math (To avoid unwanted rounding).
    // -1: max fps available.
    private static readonly GENERAL_FPS: number = 10;
    
    private general_deltaS: number = 0;
    private general_deltaMS: number = 0;
        
    public constructor() {
        super();
        
        //void
    }

    public distributeLoop(deltaS: number, deltaMS: number): void {
        super.distributeLoop(deltaS, deltaMS);
    
        this.general_deltaS += deltaS;
        this.general_deltaMS += deltaMS;
        if(LoopDistributionGeneral.GENERAL_FPS == -1 || this.general_deltaS >= (1 / LoopDistributionGeneral.GENERAL_FPS)) {
            this.appLoopGeneral(this.general_deltaS, this.general_deltaMS);
    
            this.general_deltaS = 0;
            this.general_deltaMS = 0;
        }
    }
    
    private appLoopGeneral(deltaS: number, deltaMS: number): void {
        let listIndex: number = LoopListTypes.LOOP_INDEX_GENERAL;
    
        let rootLoopUserNode: ILoopUserBase_General = (<ILoopUserBase_General>this.rootComponent.getFloatingLoopUser(listIndex));
        if(rootLoopUserNode == null) {
            return ;
        }
        
        let currentLoopUserNode: ILoopUserBase_General = rootLoopUserNode;
        
        do {
            if(!currentLoopUserNode.getIsDisposed()) {
                currentLoopUserNode.appLoop_General(deltaS, deltaMS);
            }
    
            let nextLoopUserNode: ILoopUserBase_General = currentLoopUserNode.getNextLoopUserNode<ILoopUserBase_General>(listIndex);
    
            if(currentLoopUserNode.getIsDisposed()) {
                currentLoopUserNode.removeLoopUserNode(listIndex);
                
                // If removing the root node, save the next one as root.
                // This is required for list end detection.
                if(currentLoopUserNode == rootLoopUserNode) {
                    rootLoopUserNode = nextLoopUserNode;
                }
            }
            
            currentLoopUserNode = nextLoopUserNode;
        }while(currentLoopUserNode != rootLoopUserNode);
    }
    
}
