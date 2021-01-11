"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const lokijs = require("lokijs");
const fs = require("fs");
class DbFileSaveAdapter extends lokijs.LokiFsAdapter {
    constructor() {
        super();
        //void
    }
    /**
       * loadDatabase() - Load data from file, will throw an error if the file does not exist
       * @param dbname - the filename of the database to load
       * @param callback - the callback to handle the result
       */
    loadDatabase(dbname, callback) {
        var self = this;
        fs.stat(dbname, function (err, stats) {
            if (!err && stats.isFile()) {
                fs.readFile(dbname, {
                    encoding: 'utf8'
                }, function readFileCallback(err, data) {
                    if (err) {
                        callback(new Error(err.toString()));
                    }
                    else {
                        callback(data);
                    }
                });
            }
            else {
                callback(null);
            }
        });
    }
    /**
     * saveDatabase() - save data to file, will throw an error if the file can't be saved
     * might want to expand this to avoid dataloss on partial save
     * @param dbname - the filename of the database to load
     * @param callback - the callback to handle the result
     */
    saveDatabase(dbname, dbstring, callback) {
        var self = this;
        var tmpdbname = dbname + '.temp';
        fs.writeFile(tmpdbname, dbstring, function writeFileCallback(err) {
            if (err) {
                callback(new Error(err.toString()));
            }
            else {
                var backupdbname = dbname + ".backup_previous";
                fs.rename(dbname, backupdbname, function saveDatabasecallback(err) { });
                fs.rename(tmpdbname, dbname, callback);
            }
        });
    }
    /**
     * deleteDatabase() - delete the database file, will throw an error if the
     * file can't be deleted
     * @param dbname - the filename of the database to delete
     * @param callback - the callback to handle the result
     */
    deleteDatabase(dbname, callback) {
        fs.unlink(dbname, function deleteDatabaseCallback(err) {
            if (err) {
                callback(new Error(err.toString()));
            }
            else {
                callback();
            }
        });
    }
}
exports.DbFileSaveAdapter = DbFileSaveAdapter;
//# sourceMappingURL=DbFileSaveAdapter.js.map