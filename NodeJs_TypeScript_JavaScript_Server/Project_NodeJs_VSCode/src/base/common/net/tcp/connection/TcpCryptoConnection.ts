import { TcpRequestBlocksConnection } from './TcpRequestBlocksConnection';

import * as net from "net";

import { StreamBuffer } from "base/buffer/StreamBuffer";
import { NetMsgsBlock } from 'base/common/net/NetMsgsBlock';
import { IEncryptCipher } from 'base/crypto/cipher/IEncryptCipher';
import { IDecryptCipher } from 'base/crypto/cipher/IDecryptCipher';

export class TcpCryptoConnection extends TcpRequestBlocksConnection {

  public encryptionCipher: IEncryptCipher = null;
  public decryptionCipher: IDecryptCipher = null;

  private isEnabledAutoCryptoRead: boolean = false;
  private isEnabledAutoCryptoWrite: boolean = false;
  
  public constructor(socket: net.Socket) {
    super(socket);
    
    //void
  }

  public enabledAutoCryptoRead(): void {
    this.isEnabledAutoCryptoRead = true;
}

public enabledAutoCryptoWrite(): void {
    this.isEnabledAutoCryptoWrite = true;
}

  protected extractConfirmedReceivedBlock(blockLen: number): StreamBuffer {
    let blockNetBuffer: StreamBuffer = super.extractConfirmedReceivedBlock(blockLen);
    
    if(this.isEnabledAutoCryptoRead) {
        // Decrypt.
        // The first 2 int values are not encrypted.
        
        let blockBuffer: Buffer = blockNetBuffer.getBuffer();
        
        let cryptoOffset: number = 4 + 4;
        let cryptoDataLen: number = blockBuffer.length  - cryptoOffset;
        
        this.decryptionCipher.processBytes(
          blockBuffer.buffer, cryptoOffset, 
          blockBuffer.buffer, cryptoOffset, 
          cryptoDataLen
        );
      }
      
      return blockNetBuffer;
  }

  public sendBlock(blockDataNetBuffer: NetMsgsBlock): void {
    if(this.isEnabledAutoCryptoWrite) {
      // Encrypt.
      // The first 2 int values are not encrypted.
      
      let blockBuffer: Buffer = blockDataNetBuffer.getBuffer();
      
      let cryptoOffset: number = 4 + 4;
      let cryptoDataLen: number = blockBuffer.length  - cryptoOffset;

      this.encryptionCipher.processBytes(
          blockBuffer.buffer, cryptoOffset, 
          blockBuffer.buffer, cryptoOffset, 
          cryptoDataLen
      );
    }
    
    super.sendBlock(blockDataNetBuffer);
  }
  
}
