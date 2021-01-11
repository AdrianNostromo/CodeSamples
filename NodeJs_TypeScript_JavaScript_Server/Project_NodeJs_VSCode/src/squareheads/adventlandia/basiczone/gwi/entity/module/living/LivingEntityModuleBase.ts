import { ModuleLocal } from "../ModuleLocal";
import { EntityRemoveReason } from "localbase/gwi/entity/util/EntityRemoveReason";
import { LivingEntityTemplate } from "../../template/LivingEntityTemplate";
import { WorldEntityLocal } from "../../WorldEntityLocal";

export class LivingEntityModuleBase extends ModuleLocal {

  public static readonly ENT_REM_REASON_ENTITY_DESTROYED: EntityRemoveReason = new EntityRemoveReason("ENT_REM_REASON_ENTITY_DESTROYED");

  public readonly template: LivingEntityTemplate;

  private currentHP: number;
  
  public constructor(entity: WorldEntityLocal, template: LivingEntityTemplate) {
    super(template.moduleId, entity, false);

    this.template = template;

    // The client also initialises this value the same way. So there is no need to initialise this value and set the update flag for it.
    this.currentHP = this.template.maxHP;
  }

  public getCurrentHP(): number {
    return this.currentHP;
}

  public setCurrentHP(currentHP: number): void {
    if(this.currentHP == currentHP) {
      return ;
    }
    
    this.currentHP = currentHP;

    if(this.currentHP <= 0) {
      // This entity was destroyed, mark it for removal.
      this.getEntity().handler.markEntityForRemoval(this.getEntity(), LivingEntityModuleBase.ENT_REM_REASON_ENTITY_DESTROYED);
    }

    this.onCurrentHPChanged();
  }

  public addCurrentHP(hpAddAmount: number): void {
    this.setCurrentHP(this.currentHP + hpAddAmount);
  }

  protected onCurrentHPChanged(): void {
    //void
  }
  
}
