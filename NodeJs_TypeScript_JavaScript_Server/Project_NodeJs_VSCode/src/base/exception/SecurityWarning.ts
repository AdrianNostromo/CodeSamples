export class SecurityWarning extends Error {

  public constructor(message: string) {
    super(message);
    
    this.name = 'SecurityWarning';
  }
  
}
