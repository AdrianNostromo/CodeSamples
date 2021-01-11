
export class EntityRemoveReason {

  public static readonly CODE_DISPOSED: EntityRemoveReason = new EntityRemoveReason("CODE_DISPOSED");

  public id: string;
  
  public constructor(id: string) {
    this.id = id;
  }
  
}
