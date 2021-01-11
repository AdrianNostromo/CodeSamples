"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ModuleLocal_1 = require("../ModuleLocal");
const EntityRemoveReason_1 = require("localbase/gwi/entity/util/EntityRemoveReason");
class LivingEntityModuleBase extends ModuleLocal_1.ModuleLocal {
    constructor(entity, template) {
        super(template.moduleId, entity, false);
        this.template = template;
        // The client also initialises this value the same way. So there is no need to initialise this value and set the update flag for it.
        this.currentHP = this.template.maxHP;
    }
    getCurrentHP() {
        return this.currentHP;
    }
    setCurrentHP(currentHP) {
        if (this.currentHP == currentHP) {
            return;
        }
        this.currentHP = currentHP;
        if (this.currentHP <= 0) {
            // This entity was destroyed, mark it for removal.
            this.getEntity().handler.markEntityForRemoval(this.getEntity(), LivingEntityModuleBase.ENT_REM_REASON_ENTITY_DESTROYED);
        }
        this.onCurrentHPChanged();
    }
    addCurrentHP(hpAddAmount) {
        this.setCurrentHP(this.currentHP + hpAddAmount);
    }
    onCurrentHPChanged() {
        //void
    }
}
LivingEntityModuleBase.ENT_REM_REASON_ENTITY_DESTROYED = new EntityRemoveReason_1.EntityRemoveReason("ENT_REM_REASON_ENTITY_DESTROYED");
exports.LivingEntityModuleBase = LivingEntityModuleBase;
//# sourceMappingURL=LivingEntityModuleBase.js.map