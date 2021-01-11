"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const GWILContentInit_1 = require("./GWILContentInit");
const MathUtil_1 = require("base/util/MathUtil");
const EntityTemplatesLocal_1 = require("./config/EntityTemplatesLocal");
const Pos3D_1 = require("base/util/Pos3D");
const Quaternion3D_1 = require("base/util/Quaternion3D");
class DestroyedTreeInformation {
    constructor(treeId, posX, posY) {
        this.treeId = treeId;
        this.posX = posX;
        this.posY = posY;
    }
}
class GWILForestGrower extends GWILContentInit_1.GWILContentInit {
    constructor(handler, config) {
        super(handler, config);
        // On init. Grow forest to max saturation.
        // On game loop. Grow needed trees until saturation is reached.
        // Trees have a minimum mandaory saturation (eg: 5 trees).
        // Trees have a maximum extra saturation (eg: 20 trees).
        // Mandatory trees grow instantly when needed.
        // Extra trees grow after a delay of 30 seconds.
        /// The delayed tree
        this.destroyedTreesList = [];
        //void
    }
    create_content() {
        super.create_content();
        this.createForest();
    }
    createForest() {
        // Do a grid slots enumeration with diameter of 8x8 and a increment of 1x1.
        // Create a tree on each slot.
        // Add a random small offset to the position of each tree.
        let radiusMIncrements = [
            0,
            1,
            0.8,
            0.8,
            0.8
        ];
        let treesPerRadius = [
            0,
            3,
            6,
            12,
            12
        ];
        let islandGridDiameter = GWILForestGrower.meterToGrid(18) + 1;
        let islandCenterMX = GWILForestGrower.gridToMeter((islandGridDiameter - 1) / 2);
        let islandCenterMY = GWILForestGrower.gridToMeter((islandGridDiameter - 1) / 2);
        let currentLayerRadiusM = 0;
        for (let layer = 0; layer < radiusMIncrements.length; layer++) {
            currentLayerRadiusM += radiusMIncrements[layer];
            let currentLayerTreesCount = treesPerRadius[layer];
            if (currentLayerTreesCount > 0) {
                let startAngD = MathUtil_1.MathUtil.randomIntInRange(0, 360);
                let angDIncrements = 360 / currentLayerTreesCount;
                for (let i = 0; i < currentLayerTreesCount; i++) {
                    let treeAngD = startAngD + angDIncrements * i;
                    let treeRadiusM = currentLayerRadiusM;
                    let treePosMX = islandCenterMX + Math.cos(MathUtil_1.MathUtil.toRadians(treeAngD)) * treeRadiusM;
                    let treePosMY = islandCenterMY + Math.sin(MathUtil_1.MathUtil.toRadians(treeAngD)) * treeRadiusM;
                    this.createEntity(EntityTemplatesLocal_1.EntityTemplatesLocal.Flora_Tree.id, "_tree_l" + layer + "_i" + i, null, new Pos3D_1.Pos3D(treePosMX, treePosMY, 0), Quaternion3D_1.Quaternion3D.fromIdt());
                }
            }
        }
    }
    gameLoop_simulation(deltaS, deltaMS) {
        super.gameLoop_simulation(deltaS, deltaMS);
        //asdz;
    }
}
exports.GWILForestGrower = GWILForestGrower;
//# sourceMappingURL=GWILForestGrower.js.map