"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const LogicError_1 = require("base/exception/LogicError");
class EntityTemplate {
    constructor(id, tagsList) {
        this.moduleTemplatesMap = new Map();
        this.moduleTemplatesList = [];
        this.id = id;
        this.tagsList = tagsList;
        EntityTemplate.entriesList.push(this);
        EntityTemplate.entriesMap.set(this.id, this);
    }
    setModuleTemplate(moduleTeplate) {
        let moduleName = moduleTeplate.moduleId;
        if (this.moduleTemplatesMap.has(moduleName)) {
            throw new LogicError_1.LogicError("EntityTemplate.setSubTemplate.1.");
        }
        this.moduleTemplatesMap.set(moduleName, moduleTeplate);
        this.moduleTemplatesList.push(moduleTeplate);
        return this;
    }
    hasModuleTemplate(moduleName) {
        if (this.moduleTemplatesMap.has(moduleName) && this.moduleTemplatesMap.get(moduleName) != null) {
            return true;
        }
        return false;
    }
    getModuleTemplate(moduleName) {
        if (this.hasModuleTemplate(moduleName)) {
            return this.moduleTemplatesMap.get(moduleName);
        }
        return null;
    }
    hasTagsList(tagsList) {
        for (let i = 0; i < tagsList.length; i++) {
            let tagEntry = tagsList[i];
            if (!this.hasTag(tagEntry)) {
                return false;
            }
        }
        return true;
    }
    hasTag(tag) {
        if (this.tagsList != null) {
            for (let i = 0; i < this.tagsList.length; i++) {
                if (this.tagsList[i] == tag) {
                    return true;
                }
            }
        }
        return false;
    }
}
EntityTemplate.entriesList = [];
EntityTemplate.entriesMap = new Map();
exports.EntityTemplate = EntityTemplate;
//# sourceMappingURL=EntityTemplate.js.map