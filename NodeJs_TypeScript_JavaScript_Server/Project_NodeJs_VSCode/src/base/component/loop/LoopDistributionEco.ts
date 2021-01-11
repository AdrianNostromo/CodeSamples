import { LoopDistributionGeneral } from "./LoopDistributionGeneral";

import { ILoopUserBase_Eco } from "./list/ILoopUserBase_Eco";
import { LoopListTypes } from "./config/LoopListTypes";

export class LoopDistributionEco extends LoopDistributionGeneral {

    // Must be float because it is used in float math (To avoid unwanted rounding).
    // -1: max fps available.
    private static readonly ECO_FPS: number = 2;
    
    private eco_deltaS: number = 0;
    private eco_deltaMS: number = 0;
    
    public constructor() {
        super();
        
        //void
    }

    public distributeLoop(deltaS: number, deltaMS: number): void {
        super.distributeLoop(deltaS, deltaMS);
    
        this.eco_deltaS += deltaS;
        this.eco_deltaMS += deltaMS;
        if(LoopDistributionEco.ECO_FPS == -1 || this.eco_deltaS >= (1 / LoopDistributionEco.ECO_FPS)) {
            this.appLoopEco(this.eco_deltaS, this.eco_deltaMS);
        
            this.eco_deltaS = 0;
            this.eco_deltaMS = 0;
        }
    }
    
    private appLoopEco(deltaS: number, deltaMS: number): void {
        let listIndex: number = LoopListTypes.LOOP_INDEX_ECO;
    
        let rootLoopUserNode: ILoopUserBase_Eco = (<ILoopUserBase_Eco>this.rootComponent.getFloatingLoopUser(listIndex));
        if(rootLoopUserNode == null) {
            return ;
        }
    
        let currentLoopUserNode: ILoopUserBase_Eco = rootLoopUserNode;
    
        do {
            if(!currentLoopUserNode.getIsDisposed()) {
                currentLoopUserNode.appLoop_Eco(deltaS, deltaMS);
            }
        
            let nextLoopUserNode: ILoopUserBase_Eco = currentLoopUserNode.getNextLoopUserNode<ILoopUserBase_Eco>(listIndex);
        
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
