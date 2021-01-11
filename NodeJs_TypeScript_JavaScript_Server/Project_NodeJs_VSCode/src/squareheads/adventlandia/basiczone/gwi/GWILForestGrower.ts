import { GWILContentInit } from "./GWILContentInit";
import { IHandler_GWI } from "localbase/gwi/IHandler_GWI";
import { DTGWIConfigBasicZone } from "./util/DTGWIConfigBasicZone";
import { MathUtil } from "base/util/MathUtil";
import { EntityTemplatesLocal } from "./config/EntityTemplatesLocal";
import { Pos3D } from "base/util/Pos3D";
import { Quaternion3D } from "base/util/Quaternion3D";

class DestroyedTreeInformation {

  public treeId: string;

  public posX: number;
  public posY: number;

  public constructor(treeId: string, posX: number, posY: number) {
    this.treeId = treeId;
    
    this.posX = posX;
    this.posY = posY;
  }

}

export abstract class GWILForestGrower extends GWILContentInit {

  // On init. Grow forest to max saturation.
  // On game loop. Grow needed trees until saturation is reached.
  // Trees have a minimum mandaory saturation (eg: 5 trees).
  // Trees have a maximum extra saturation (eg: 20 trees).
  // Mandatory trees grow instantly when needed.
  // Extra trees grow after a delay of 30 seconds.
  /// The delayed tree

  private destroyedTreesList: DestroyedTreeInformation[] = [];

  public constructor(handler: IHandler_GWI, config: DTGWIConfigBasicZone) {
    super(handler, config);
  
    //void
  }
  
  protected create_content(): void {
    super.create_content();

    this.createForest();
  }

  private createForest(): void {
    // Do a grid slots enumeration with diameter of 8x8 and a increment of 1x1.
    // Create a tree on each slot.
    // Add a random small offset to the position of each tree.
    
    let radiusMIncrements: number[] = [
        0,
        1,
        0.8,
        0.8,
        0.8
    ];
    let treesPerRadius: number[] = [
        0,
        3,
        6,
        12,
        12
    ];

    let islandGridDiameter: number = GWILForestGrower.meterToGrid(18) + 1;
    let islandCenterMX: number = GWILForestGrower.gridToMeter((islandGridDiameter - 1) / 2);
    let islandCenterMY: number = GWILForestGrower.gridToMeter((islandGridDiameter - 1) / 2);

    let currentLayerRadiusM: number = 0;
    for(let layer: number=0;layer < radiusMIncrements.length;layer++) {
        currentLayerRadiusM += radiusMIncrements[layer];
        
        let currentLayerTreesCount: number = treesPerRadius[layer];
        if(currentLayerTreesCount > 0) {
            let startAngD: number = MathUtil.randomIntInRange(0, 360);
            let angDIncrements: number = 360 / currentLayerTreesCount;

            for (let i: number = 0; i < currentLayerTreesCount; i++) {
                let treeAngD: number = startAngD + angDIncrements * i;
                let treeRadiusM: number = currentLayerRadiusM;

                let treePosMX: number = islandCenterMX + Math.cos(MathUtil.toRadians(treeAngD)) * treeRadiusM;
                let treePosMY: number = islandCenterMY + Math.sin(MathUtil.toRadians(treeAngD)) * treeRadiusM;

                this.createEntity(
                  EntityTemplatesLocal.Flora_Tree.id,
                    "_tree_l" + layer + "_i" + i,
                    null,
                    new Pos3D(
                        treePosMX,
                        treePosMY,
                        0
                    ),
                    Quaternion3D.fromIdt()
                );
            }
        }
    }
}

  protected gameLoop_simulation(deltaS: number, deltaMS: number): void {
    super.gameLoop_simulation(deltaS, deltaMS);

    //asdz;
  }

}
