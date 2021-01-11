export class DataFormatError extends Error {

  public constructor(message: string) {
    super(message);
    
    this.name = 'DataFormatError';
  }
  
}
