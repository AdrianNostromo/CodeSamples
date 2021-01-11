"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicModuleTemplate_1 = require("localbase/gwi/entity/template/BasicModuleTemplate");
class MovingEntityTemplate extends BasicModuleTemplate_1.BasicModuleTemplate {
    constructor(moveSpeedS) {
        super(MovingEntityTemplate.MODULE_ID, true);
        this.moveSpeedS = moveSpeedS;
    }
}
MovingEntityTemplate.MODULE_ID = "movingEntity";
exports.MovingEntityTemplate = MovingEntityTemplate;
//# sourceMappingURL=MovingEntityTemplate.js.map