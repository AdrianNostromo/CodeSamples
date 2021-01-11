export class LogicException extends Error {

  public constructor(message: string) {
    super(message);
    
    this.name = 'LogicException';
  }
  
}
