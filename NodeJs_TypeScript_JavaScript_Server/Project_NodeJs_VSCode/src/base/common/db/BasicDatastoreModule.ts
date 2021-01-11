import { BasicAppComponent } from "base/component/BasicAppComponent";

import { IApp } from "base/app/IApp";

import { DatastoreWrapper } from "./DatastoreWrapper";
import { ComponentStatus } from "base/common/util/ComponentStatus";
import { FileUtil } from "base/common/file/util/FileUtil";
import * as lokijs from "lokijs";
import { LogicError } from "base/exception/LogicError";

export abstract class BasicDatastoreModule extends BasicAppComponent {

  protected dbInstance: DatastoreWrapper = null;
  
  public constructor(app: IApp) {
    super(app);
  }
  
  public componentStatusChanged(componentStatus: ComponentStatus): void {
    super.componentStatusChanged(componentStatus);

    if(componentStatus == ComponentStatus.STARTING) {
      this.dbInstance = new DatastoreWrapper(
        this.getDbPath(),

        this.onDbError.bind(this),
        this.onDbLoadFinished.bind(this),
        this.onInitDbColleactions.bind(this)
      ).load();
    }
  }
  
  private onDbError(target: DatastoreWrapper, err: Error): void {
    this.getApp().getLog().error(err.toString());

    this.getApp().getErrorHandler().error(err.toString());
  }
  
  private onDbLoadFinished(target: DatastoreWrapper): void {
    this.onDbLoaded();
  }
  
  //extendable
  protected onDbLoaded(): void {
    //void
  }
  
  protected abstract getDbPath(): string;
  
  //extendable
  protected onInitDbColleactions(): void {
    //void
  }

  public isComponentTransientStateFinished(componentStatus: ComponentStatus): boolean {
    let ret: boolean = super.isComponentTransientStateFinished(componentStatus);
    
    if(componentStatus == ComponentStatus.STARTING) {
      if(!this.dbInstance.isDbLoadFinished) {
        ret = false;
      }
    }
    
    return ret;
  }
  
  protected saveDatabase(): void {
    this.dbInstance.saveDatabase();
  }
  
  protected factoryInitializeNewDatabase(): void {
    let dbFilePath: string = this.getDbPath();

    let exists: boolean;
    try {
      exists = FileUtil.fileExistsSync(dbFilePath)
    }catch(x) {
      throw new LogicError("Db initialize error. Db file check file system error.");
    }
    if(exists) {
      throw new LogicError("Db initialize error. Db file already exists.");
    }

    let db: Loki = new lokijs(dbFilePath);

    db = new lokijs(dbFilePath);

    this.factoryInitializeNewCollections(db);

    db.saveDatabase();
  }

  protected factoryInitializeNewCollections(db: Loki): void {
    //void
  }

}
