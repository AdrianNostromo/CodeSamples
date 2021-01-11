"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const fs = require("fs");
class FileUtil {
    static folderExistsSync(path) {
        return FileUtil.existsSync(path);
    }
    static createFolderSync(path) {
        try {
            fs.mkdirSync(path);
        }
        catch (x) {
            if (x.code == 'EEXIST') {
                // Folder already exists.
            }
            throw x;
        }
    }
    static deleteFolderSync(path) {
        try {
            if (FileUtil.existsSync(path)) {
                fs.readdirSync(path).forEach(function (file, index) {
                    let curPath = path + "/" + file;
                    if (fs.lstatSync(curPath).isDirectory()) { // recurse
                        FileUtil.deleteFolderSync(curPath);
                    }
                    else { // delete file
                        FileUtil.deleteFileSync(curPath);
                    }
                });
                fs.rmdirSync(path);
            }
        }
        catch (x) {
            throw x;
        }
    }
    static moveFolderSync(srcPath, destPath) {
        FileUtil.moveFileSync(srcPath, destPath);
    }
    static fileExistsSync(path) {
        return FileUtil.existsSync(path);
    }
    static existsSync(path) {
        let fileStats = null;
        try {
            fileStats = fs.statSync(path);
        }
        catch (x) {
            if (x.code == 'ENOENT') {
                // File does not exist.
                return false;
            }
            else {
                // Unknow error occured.
                throw x;
            }
        }
        return true;
    }
    static moveFileSync(srcPath, destPath) {
        try {
            fs.renameSync(srcPath, destPath);
        }
        catch (x) {
            throw x;
        }
    }
    static deleteFileSync(path) {
        try {
            fs.unlinkSync(path);
        }
        catch (x) {
            if (x.code == 'ENOENT') {
                // File not found.
                //void
            }
            throw x;
        }
    }
}
exports.FileUtil = FileUtil;
//# sourceMappingURL=FileUtil.js.map