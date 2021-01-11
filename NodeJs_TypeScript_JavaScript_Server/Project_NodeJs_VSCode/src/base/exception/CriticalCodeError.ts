export class CriticalCodeError extends Error {

  public constructor(message: string) {
    super(message);
    
    this.name = 'CriticalCodeError';
  }
  
}
