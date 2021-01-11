let Module: NodeRequire = require('module');
let originalRequire: any = Module.prototype.require;

// Refactor the require function to allow for absolute and relative paths.
Module.prototype.require = function(){
  if((arguments[0].indexOf(".") != 0 && arguments[0].indexOf("/") != 0) && arguments[0].indexOf("/") >= 0) {
    arguments[0] = __dirname + "/" + arguments[0];
  }

  return originalRequire.apply(this, arguments);
};

import { ComponentType } from "base/common/ComponentType";
import { FileUtil } from "base/common/file/util/FileUtil";

import { AppDirs } from "base/common/util/AppDirs";
import { AppConfig } from "base/common/config/AppConfig";

import { AppLocal as AppServerManager } from "servermanager/app/AppLocal";
import { AppLocal as AppNodeController } from "nodecontroller/app/AppLocal";
import { AppLocal as AppLogManager } from "logmanager/app/AppLocal";
import { AppLocal as AppSquareHeadsManager } from "squareheads/app/AppLocal";
import { ComponentStatus } from "base/common/util/ComponentStatus";
import { App } from "base/app/App";

// import { Salsa20 } from "base/crypto/rawalgorithm/Salsa20";
// import { CryptoUtil } from "base/crypto/util/CryptoUtil";
// import { IDecryptCipher } from "base/crypto/cipher/IDecryptCipher";
// import { IEncryptCipher } from "base/crypto/cipher/IEncryptCipher";
// import { DecryptCipher } from "base/crypto/cipher/DecryptCipher";

//asd_tst;
/*import * as crypto from "crypto";
import { CryptoUtil } from "base/util/CryptoUtil";*/
/*
interface IA {
    _IA: true
}

class A implements IA {
    public _IA: true = true;
}

class B {
}
*/
export class Main {

  public app: App = null;

  private _lastLoopTimeMS: number = undefined;
  private _bf_timerLoop: () => void = null;

  public constructor() {
    //void
  }

  // START : asd_tst;
  /*
  private tst(): void {
    let buff: Buffer = Buffer.alloc(256);
    console.log("asd1");
    for(let i: number=0;i<256;i++) {
      buff.writeUInt8(i, i);
      let targetByte: number = buff.readUInt8(i);
      buff.writeUInt8(targetByte, i);
      targetByte = buff.readUInt8(i);
      //destBitMap.writeUInt8(targetByte, destByteIndex);
      console.log(targetByte);
    }
    console.log("asd2");
  }
  /*
    private implementsInterface(object: Object, interfaceName: string): boolean {
        return object.hasOwnProperty("_" + interfaceName);
    }

    private tst(): void {
        let a: A = new A();
        let b: B = new B();
        let b1: boolean = a instanceof A;
        let b2: boolean = this.implementsInterface(a, "IA");
        let b3: boolean = this.implementsInterface(b, "IA");
        let s1: string = typeof a;
        let sx: string = typeof a;


    }*/
  //asd_tst;
  /*private tst_creatCipher(): crypto.Cipher {
    let keyString: string = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    let unCutKeyBytes: Buffer = Buffer.allocUnsafe(Buffer.byteLength(keyString, 'utf8'));
    unCutKeyBytes.write(keyString);
    
    let key: Buffer = Buffer.alloc(128 / 8);
    unCutKeyBytes.copy(key);

    let iv: Buffer = Buffer.alloc(128 / 8);
    let cipher: crypto.Cipher = crypto.createCipheriv(
      'aes-128-cfb8', 
      key, 
      iv
    );

    return cipher;
  }
  
  private tst_creatDecipher(): crypto.Decipher {
    let keyString: string = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    let unCutKeyBytes: Buffer = Buffer.allocUnsafe(Buffer.byteLength(keyString, 'utf8'));
    unCutKeyBytes.write(keyString);
    
    let key: Buffer = Buffer.alloc(128 / 8);
    unCutKeyBytes.copy(key);

    let iv: Buffer = Buffer.alloc(128 / 8);
    let decipher: crypto.Decipher = crypto.createDecipheriv(
      'aes-128-cfb8', 
      key, 
      iv
    );

    return decipher;
  }
  
  private tst(): void {
    let cipher: crypto.Cipher = this.tst_creatCipher();
    let decipher: crypto.Decipher = this.tst_creatDecipher();

    let a1: string = CryptoUtil.encryptStringToBase64("aaa", cipher);
    let a2: string = CryptoUtil.encryptStringToBase64("aaa", cipher);
    let a3: string = CryptoUtil.encryptStringToBase64("aaa", cipher);
    let a4: string = CryptoUtil.encryptStringToBase64("aaa", cipher);

    let b1: string = CryptoUtil.decryptBase64ToString(a1, decipher);
    let b2: string = CryptoUtil.decryptBase64ToString(a2, decipher);
    let b3: string = CryptoUtil.decryptBase64ToString(a3, decipher);
    let b4: string = CryptoUtil.decryptBase64ToString(a4, decipher);
  }*/

//   private tst(): void {
//     let decryptionKeyString: string = "12a4b8dc40ffa121c0bbcf895dff9cf2";
//     let keyStringBuffer: Buffer = Buffer.from(decryptionKeyString, "utf8");
//     let key: Uint8Array = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 32);
//     let iv: Uint8Array = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 8);

//     // Initialize encription.
//     let decryptionCipher: IDecryptCipher = CryptoUtil.newDecryptCipher(
//         CryptoUtil.Algorithms_Salsa20, 
//         key, iv
//     );

//     let encryptionKeyString: string = "12a4b8dc40ffa121c0bbcf895dff9cf2";
//     keyStringBuffer = Buffer.from(encryptionKeyString, "utf8");
//     key = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 32);
//     iv = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 8);

//     // Initialize encription.
//     let encryptionCipher: IEncryptCipher = CryptoUtil.newEncryptCipher(
//         CryptoUtil.Algorithms_Salsa20, 
//         key, iv
//     );

//     let t1: string = "DOOD";
//     let t2: string = encryptionCipher.encryptStringToBase64(t1);
//     let t3: string = decryptionCipher.decryptBase64ToString(t2);
//   }

  // END : asd_tst;

  public create(): void {
    //asd_tst;
    // this.tst();

    let componentTypeName: string = null;
    let indexOnNode: number = null;
    process.argv.forEach(function (val, index, array) {
      if(index == 2) {
        componentTypeName = val;
      }else if(index == 3) {
        indexOnNode = Number(val);
      }
    });
    if(!componentTypeName) {
      console.log('Arguments error. Component type missing');
      
      process.exit(-1);
    }
    if(!indexOnNode) {
      console.log('Arguments error. Index on node missing');
      
      process.exit(-2);
    }
    
    let componentType: ComponentType = ComponentType.fromName(componentTypeName);
    if(!componentType) {
      console.log('Arguments error. Root dir missing');
      
      process.exit(-3);
    }
    
    let debug: boolean = process.execArgv.indexOf('--debug') !== -1;
    
    let componentsRootDir: string = './../components/';
    let componentDir: string = componentsRootDir + componentType.name + '_' + indexOnNode + '/';
    let dataDir: string = './../data/' + componentType.name + "/";
    
    try {
      if(!FileUtil.folderExistsSync(componentsRootDir)) {
        // The components root folder must be manually created.
        console.log('Error. Component root folder not found. It must be manually created.');
        
        process.exit(-4);
        
        return ;
      }
      
      if(FileUtil.folderExistsSync(componentDir)) {
        FileUtil.deleteFolderSync(componentDir);
      }
      FileUtil.createFolderSync(componentDir);

      if(!FileUtil.folderExistsSync(dataDir)) {
        FileUtil.createFolderSync(dataDir);
      }
    }catch(x) {
      console.log('Error. Component dir folders error.');
      
      process.exit(-4);
      
      return ;
    }
    
    let dirs: AppDirs = new AppDirs(
      componentDir,
      dataDir
    );
    
    switch (componentType) {
      case ComponentType.SERVER_MANAGER:
        this.app = new AppServerManager(componentType, dirs);
        break;
      
      case ComponentType.NODE_CONTROLLER:
        this.app = new AppNodeController(componentType, dirs);
        break;
      
      case ComponentType.LOG_MANAGER:
        this.app = new AppLogManager(componentType, dirs);
        break;

      case ComponentType.SQUARE_HEADS_MANAGER:
        this.app = new AppSquareHeadsManager(componentType, dirs);
        break;
      
      default:
        console.log('Compo=nent creation implementation error. Component app class creation code missing');
        
        process.exit(-5);

        break;
    }
    
    this.app.reservedCreate();
    this.app.startApp();

    this._bf_timerLoop = this.timerLoop.bind(this);
    setTimeout(this._bf_timerLoop, 1000 / AppConfig.LOOP_FPS);
  }

  private timerLoop(): void {
    let tm: number = (new Date).getTime();
    
    if(this._lastLoopTimeMS !== undefined) {
      // Limit delta time tick to 0.5s;
      let deltaMS: number = Math.min(500, tm - this._lastLoopTimeMS);
      let deltaS: number = deltaMS / 1000;
      
      this.app.loopDistribution.distributeLoop(deltaS, deltaMS);
    }
    
    this._lastLoopTimeMS = tm;
    
    setTimeout(this._bf_timerLoop, 1000 / AppConfig.LOOP_FPS);
    
    if(this.app.getComponentStatus() == ComponentStatus.OFFLINE) {
        console.log("Component offline. Exiting.");

        process.exit(0);
    }
  }
}

let main: Main = new Main();
main.create();
