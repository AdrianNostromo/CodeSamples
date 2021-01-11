import * as lokijs from "lokijs";
import * as fs from "fs";

export class DbFileSaveAdapter extends lokijs.LokiFsAdapter {

  public constructor() {
    super();
      
    //void
  }

  /**
     * loadDatabase() - Load data from file, will throw an error if the file does not exist
     * @param dbname - the filename of the database to load
     * @param callback - the callback to handle the result
     */
    public loadDatabase(dbname: string, callback: (data: any | Error) => void): void {
        var self = this;

        fs.stat(dbname, function (err, stats) {
            if (!err && stats.isFile()) {
                fs.readFile(dbname, {
                    encoding: 'utf8'
                }, function readFileCallback(err, data) {
                    if (err) {
                        callback(new Error(err.toString()));
                    } else {
                        callback(data);
                    }
                });
            }else {
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
    public saveDatabase(dbname: string, dbstring: string | Uint8Array, callback: (err?: Error | null) => void): void {
        var self = this;
        var tmpdbname = dbname + '.temp';
        fs.writeFile(tmpdbname, dbstring, function writeFileCallback(err) {
            if (err) {
                callback(new Error(err.toString()));
            } else {
                var backupdbname = dbname + ".backup_previous";
                fs.rename(dbname, backupdbname, function saveDatabasecallback(err) {/* void */});
                fs.rename(tmpdbname,dbname,callback);
            }
        });
    }

    /**
     * deleteDatabase() - delete the database file, will throw an error if the
     * file can't be deleted
     * @param dbname - the filename of the database to delete
     * @param callback - the callback to handle the result
     */
    public deleteDatabase(dbname: string, callback: (err?: Error | null) => void): void {
        fs.unlink(dbname, function deleteDatabaseCallback(err) {
            if (err) {
                callback(new Error(err.toString()));
            } else {
                callback();
            }
        });
    }

}
