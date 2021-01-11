import * as fs from "fs";

export class FileUtil {

  public static folderExistsSync(path: string): boolean {
    return FileUtil.existsSync(path);
  }
  
  public static createFolderSync(path: string): void {
    try {
      fs.mkdirSync(path);
    }catch(x) {
      if(x.code == 'EEXIST') {
        // Folder already exists.
      }
      
      throw x;
    }
  }
  
  public static deleteFolderSync(path: string): void {
    try {
      if(FileUtil.existsSync(path) ) {
        fs.readdirSync(path).forEach(function(file, index){
          let curPath: string = path + "/" + file;
          if(fs.lstatSync(curPath).isDirectory()) { // recurse
            FileUtil.deleteFolderSync(curPath);
          } else { // delete file
            FileUtil.deleteFileSync(curPath);
          }
        });
        fs.rmdirSync(path);
      }
    }catch(x) {
      throw x;
    }
  }
  
  public static moveFolderSync(srcPath: string, destPath: string): void {
    FileUtil.moveFileSync(srcPath, destPath);
  }
  
  public static fileExistsSync(path: string): boolean {
    return FileUtil.existsSync(path);
  }
  
  public static existsSync(path: string): boolean {
    let fileStats: fs.Stats = null;
    try {
      fileStats = fs.statSync(path);
    }catch(x) {
      if(x.code == 'ENOENT') {
        // File does not exist.
        
        return false;
      }else {
        // Unknow error occured.
        
        throw x;
      }
    }
    
    return true;
  }
  
  public static moveFileSync(srcPath: string, destPath: string): void {
    try {
      fs.renameSync(srcPath, destPath);
    }catch(x) {
      throw x;
    }
  }
  
  public static deleteFileSync(path: string): void {
    try {
      fs.unlinkSync(path);
    }catch(x) {
      if(x.code == 'ENOENT') {
        // File not found.
        
        //void
      }
      
      throw x;
    }
  }
  
}
