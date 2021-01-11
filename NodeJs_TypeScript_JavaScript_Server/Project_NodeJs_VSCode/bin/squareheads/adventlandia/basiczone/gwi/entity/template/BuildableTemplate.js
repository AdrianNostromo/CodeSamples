"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicModuleTemplate_1 = require("localbase/gwi/entity/template/BasicModuleTemplate");
class BuildableTemplate extends BasicModuleTemplate_1.BasicModuleTemplate {
    constructor(buildCostList) {
        super(BuildableTemplate.MODULE_ID, false);
        this.buildCostList = buildCostList;
    }
}
BuildableTemplate.MODULE_ID = "buildable";
exports.BuildableTemplate = BuildableTemplate;
//# sourceMappingURL=BuildableTemplate.js.map