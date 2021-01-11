import * as lokijs from "lokijs";
import { FileUtil } from "base/common/file/util/FileUtil";
import { DbFileSaveAdapter } from "base/common/db/util/DbFileSaveAdapter";

/*
'Find' Operators
The primary operators currently supported are : 
$eq - filter for document(s) with property of (strict) equality
$dteq - filter for document(s) with date property equal to provided date value
$gt - filter for document(s) with property greater than provided value
$gte - filter for document(s) with property greater or equal to provided value
$lt - filter for document(s) with property less than provided value
$lte - filter for document(s) with property less than or equal to provided value
$ne - filter for document(s) with property not equal to provided value
$regex - filter for document(s) with property matching provided regular expression
$in - filter for document(s) with property matching any of the provided array values.
$contains - filter for document(s) with property containing the provided value
$containsAny - filter for document(s) with property containing any of the provided values
$containsNone - filter for documents(s) with property containing none of the provided values
$and - filter for documents which meet all nested subexpressions
$or - filter for documents which meet any of the nested subexpressions
*/

export class DatastoreWrapper {

  private dbFilePath: string;

  private cbError: (target: DatastoreWrapper, err: Error) => void;
  private cbDbLoaded: (target: DatastoreWrapper) => void;
  private cbInitDbColleactions: (target: DatastoreWrapper) => void;

  protected db: Loki = null;
  public isDbLoadFinished: boolean = false;

  public constructor(
    dbFilePath: string,
    
    cbError: (target: DatastoreWrapper, err: Error) => void,
    cbDbLoaded: (target: DatastoreWrapper) => void,
    cbInitDbColleactions: (target: DatastoreWrapper) => void
  ) {
    this.dbFilePath = dbFilePath;
    
    this.cbError = cbError;
    this.cbDbLoaded = cbDbLoaded;
    this.cbInitDbColleactions = cbInitDbColleactions;
  }

  public load(): DatastoreWrapper {
    try {
      if(!FileUtil.fileExistsSync(this.dbFilePath)) {
        this.onError(new Error("Database file not found."));
        
        throw new Error("Database file not found.");
      }
    }catch(x) {
      this.onError(new Error("Database file access error 1."));
      
      throw new Error("Database file access error 1.");
    }
    
    let dbOpts: any = {
        adapter: new DbFileSaveAdapter()
    };

    this.db = new lokijs(this.dbFilePath, dbOpts);
    this.db.loadDatabase({}, this.onDbLoaded.bind(this));

    return this;
  }
  
  private onError(err: Error): void {
    if(this.cbError != null) {
      this.cbError(this, err);
    }
  }
  
  private onDbLoaded(err: Error): void {
    if(err) {
      this.onError(new Error("Error 1."));
      
      return ;
    }
    
    this.isDbLoadFinished = true;
    this.onInitDbColleactions();

    if(this.cbDbLoaded != null) {
      this.cbDbLoaded(this);
    }
  }
  
  protected onInitDbColleactions(): void {
    if(this.cbInitDbColleactions != null) {
      this.cbInitDbColleactions(this);
    }
  }

  public saveDatabase(): void {
    this.db.saveDatabase();
  }

  public getCollection<T extends Object>(collectionName: string): Collection<T> {
    return this.db.getCollection<T>(collectionName);
  }
  
}
