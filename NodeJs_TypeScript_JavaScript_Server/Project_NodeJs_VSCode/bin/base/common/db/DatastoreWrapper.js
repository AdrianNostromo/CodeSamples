"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const lokijs = require("lokijs");
const FileUtil_1 = require("base/common/file/util/FileUtil");
const DbFileSaveAdapter_1 = require("base/common/db/util/DbFileSaveAdapter");
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
class DatastoreWrapper {
    constructor(dbFilePath, cbError, cbDbLoaded, cbInitDbColleactions) {
        this.db = null;
        this.isDbLoadFinished = false;
        this.dbFilePath = dbFilePath;
        this.cbError = cbError;
        this.cbDbLoaded = cbDbLoaded;
        this.cbInitDbColleactions = cbInitDbColleactions;
    }
    load() {
        try {
            if (!FileUtil_1.FileUtil.fileExistsSync(this.dbFilePath)) {
                this.onError(new Error("Database file not found."));
                throw new Error("Database file not found.");
            }
        }
        catch (x) {
            this.onError(new Error("Database file access error 1."));
            throw new Error("Database file access error 1.");
        }
        let dbOpts = {
            adapter: new DbFileSaveAdapter_1.DbFileSaveAdapter()
        };
        this.db = new lokijs(this.dbFilePath, dbOpts);
        this.db.loadDatabase({}, this.onDbLoaded.bind(this));
        return this;
    }
    onError(err) {
        if (this.cbError != null) {
            this.cbError(this, err);
        }
    }
    onDbLoaded(err) {
        if (err) {
            this.onError(new Error("Error 1."));
            return;
        }
        this.isDbLoadFinished = true;
        this.onInitDbColleactions();
        if (this.cbDbLoaded != null) {
            this.cbDbLoaded(this);
        }
    }
    onInitDbColleactions() {
        if (this.cbInitDbColleactions != null) {
            this.cbInitDbColleactions(this);
        }
    }
    saveDatabase() {
        this.db.saveDatabase();
    }
    getCollection(collectionName) {
        return this.db.getCollection(collectionName);
    }
}
exports.DatastoreWrapper = DatastoreWrapper;
//# sourceMappingURL=DatastoreWrapper.js.map