import { LoopDistributionEco } from "./LoopDistributionEco";

import { ILoopUserBase_GameWorld } from "./list/ILoopUserBase_GameWorld";
import { LoopListTypes } from "./config/LoopListTypes";

export class LoopDistributionGameWorld extends LoopDistributionEco {

    // Must be float because it is used in float math (To avoid unwanted rounding).
    // -1: max fps available.
    private static readonly GAME_WORLD_FPS: number = 5;
    
    private gameWorld_deltaS: number = 0;
    private gameWorld_deltaMS: number = 0;
        
    public constructor() {
        super();
        
        //void
    }

    public distributeLoop(deltaS: number, deltaMS: number): void {
        super.distributeLoop(deltaS, deltaMS);
    
        this.gameWorld_deltaS += deltaS;
        this.gameWorld_deltaMS += deltaMS;
        if(LoopDistributionGameWorld.GAME_WORLD_FPS == -1 || this.gameWorld_deltaS >= (1 / LoopDistributionGameWorld.GAME_WORLD_FPS)) {
            this.appLoopGameWorld(this.gameWorld_deltaS, this.gameWorld_deltaMS);
    
            this.gameWorld_deltaS = 0;
            this.gameWorld_deltaMS = 0;
        }
    }
    
    private appLoopGameWorld(deltaS: number, deltaMS: number): void {
        let listIndex: number = LoopListTypes.LOOP_INDEX_GAME_WORLD;
    
        let rootLoopUserNode: ILoopUserBase_GameWorld = (<ILoopUserBase_GameWorld>this.rootComponent.getFloatingLoopUser(listIndex));
        if(rootLoopUserNode == null) {
            return ;
        }
    
        let currentLoopUserNode: ILoopUserBase_GameWorld = rootLoopUserNode;
    
        do {
            if(!currentLoopUserNode.getIsDisposed()) {
                currentLoopUserNode.appLoop_GameWorld(deltaS, deltaMS);
            }
        
            let nextLoopUserNode: ILoopUserBase_GameWorld = currentLoopUserNode.getNextLoopUserNode<ILoopUserBase_GameWorld>(listIndex);
        
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
