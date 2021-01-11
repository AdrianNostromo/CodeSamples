"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicModuleTemplate_1 = require("localbase/gwi/entity/template/BasicModuleTemplate");
class LivingEntityTemplate extends BasicModuleTemplate_1.BasicModuleTemplate {
    constructor(maxHP) {
        super(LivingEntityTemplate.MODULE_ID, true);
        this.maxHP = maxHP;
    }
}
LivingEntityTemplate.MODULE_ID = "livingEntity";
exports.LivingEntityTemplate = LivingEntityTemplate;
//# sourceMappingURL=LivingEntityTemplate.js.map