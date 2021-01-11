import { LogicError } from "base/exception/LogicError";
import { BasicModuleTemplate } from "./BasicModuleTemplate";

export class EntityTemplate {

  public static entriesList: EntityTemplate[] = [];
  public static entriesMap: Map<string, EntityTemplate> = new Map();

  public id: string;
  
  public tagsList: string[];
  
  public moduleTemplatesMap: Map<string, BasicModuleTemplate> = new Map();
  public moduleTemplatesList: BasicModuleTemplate[] = [];
  
  public constructor(id: string, tagsList: string[]) {
    this.id = id;

    this.tagsList = tagsList;

    EntityTemplate.entriesList.push(this);
    EntityTemplate.entriesMap.set(this.id, this);
  }

  public setModuleTemplate(moduleTeplate: BasicModuleTemplate): EntityTemplate {
    let moduleName: string = moduleTeplate.moduleId;

    if(this.moduleTemplatesMap.has(moduleName)) {
      throw new LogicError("EntityTemplate.setSubTemplate.1.");
    }

    this.moduleTemplatesMap.set(moduleName, moduleTeplate);
    this.moduleTemplatesList.push(moduleTeplate);

    return this;
  }

  public hasModuleTemplate(moduleName: string): boolean {
    if(this.moduleTemplatesMap.has(moduleName) && this.moduleTemplatesMap.get(moduleName) != null) {
      return true;
    }

    return false;
  }

  public getModuleTemplate<T extends BasicModuleTemplate>(moduleName: string): T {
    if(this.hasModuleTemplate(moduleName)) {
      return <T>this.moduleTemplatesMap.get(moduleName);
    }

    return null;
  }

  public hasTagsList(tagsList: string[]): boolean {
    for(let i: number=0;i<tagsList.length;i++) {
      let tagEntry: string = tagsList[i];
      if(!this.hasTag(tagEntry)) {
          return false;
      }
    }
    
    return true;
  }

  public hasTag(tag: string): boolean {
    if(this.tagsList != null) {
      for (let i: number = 0; i < this.tagsList.length; i++) {
          if(this.tagsList[i] == tag) {
              return true;
          }
      }
    }
    
    return false;
  }

}
