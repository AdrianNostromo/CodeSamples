
export class ResourceCost {

  public resourceTags: string[];
  public itemsCount: number;
  
  public constructor(resourceTags: string[], itemsCount: number) {
    this.resourceTags = resourceTags;
    this.itemsCount = itemsCount;
  }

}
