"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
const DatastoreWrapper_1 = require("./DatastoreWrapper");
const ComponentStatus_1 = require("base/common/util/ComponentStatus");
const FileUtil_1 = require("base/common/file/util/FileUtil");
const lokijs = require("lokijs");
const LogicError_1 = require("base/exception/LogicError");
class BasicDatastoreModule extends BasicAppComponent_1.BasicAppComponent {
    constructor(app) {
        super(app);
        this.dbInstance = null;
    }
    componentStatusChanged(componentStatus) {
        super.componentStatusChanged(componentStatus);
        if (componentStatus == ComponentStatus_1.ComponentStatus.STARTING) {
            this.dbInstance = new DatastoreWrapper_1.DatastoreWrapper(this.getDbPath(), this.onDbError.bind(this), this.onDbLoadFinished.bind(this), this.onInitDbColleactions.bind(this)).load();
        }
    }
    onDbError(target, err) {
        this.getApp().getLog().error(err.toString());
        this.getApp().getErrorHandler().error(err.toString());
    }
    onDbLoadFinished(target) {
        this.onDbLoaded();
    }
    //extendable
    onDbLoaded() {
        //void
    }
    //extendable
    onInitDbColleactions() {
        //void
    }
    isComponentTransientStateFinished(componentStatus) {
        let ret = super.isComponentTransientStateFinished(componentStatus);
        if (componentStatus == ComponentStatus_1.ComponentStatus.STARTING) {
            if (!this.dbInstance.isDbLoadFinished) {
                ret = false;
            }
        }
        return ret;
    }
    saveDatabase() {
        this.dbInstance.saveDatabase();
    }
    factoryInitializeNewDatabase() {
        let dbFilePath = this.getDbPath();
        let exists;
        try {
            exists = FileUtil_1.FileUtil.fileExistsSync(dbFilePath);
        }
        catch (x) {
            throw new LogicError_1.LogicError("Db initialize error. Db file check file system error.");
        }
        if (exists) {
            throw new LogicError_1.LogicError("Db initialize error. Db file already exists.");
        }
        let db = new lokijs(dbFilePath);
        db = new lokijs(dbFilePath);
        this.factoryInitializeNewCollections(db);
        db.saveDatabase();
    }
    factoryInitializeNewCollections(db) {
        //void
    }
}
exports.BasicDatastoreModule = BasicDatastoreModule;
//# sourceMappingURL=BasicDatastoreModule.js.map