export class SecurityError extends Error {

  public constructor(message: string) {
    super(message);
    
    this.name = 'SecurityError';
  }
  
}
