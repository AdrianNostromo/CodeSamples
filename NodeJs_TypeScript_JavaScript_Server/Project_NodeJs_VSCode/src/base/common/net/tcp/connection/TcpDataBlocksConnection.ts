import { BasicTcpConnection } from "./BasicTcpConnection";

import * as net from "net";

import { DataFormatError } from 'base/net/exception/DataFormatError';
import { NetDataLimits } from 'base/common/net/NetDataLimits';
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { NetMsgsBlock } from 'base/common/net/NetMsgsBlock';

export class TcpDataBlocksConnection extends BasicTcpConnection {

  protected currentBlockNetBuffer: StreamBuffer = null;
  
  private _lastReceivedDataBlockIndice: number = 0;
  private _sendDataBlockIndiceCounter: number = 0;

  public constructor(socket: net.Socket) {
    super(socket);
    
    //void
  }
  
  public sendBlock(blockDataNetBuffer: NetMsgsBlock): void {
    blockDataNetBuffer.lockForSend(++this._sendDataBlockIndiceCounter);
    
    if(blockDataNetBuffer.getLength() < NetDataLimits.BLOCK_LEN_MIN 
      || blockDataNetBuffer.getLength() > NetDataLimits.BLOCK_LEN_MAX 
      || blockDataNetBuffer.getOffset() != blockDataNetBuffer.getLength() 
      || blockDataNetBuffer.getBoundsLimit() != blockDataNetBuffer.getLength())
    {
      throw new Error('Msg block data shorter than the minimum block length or is longer or shorter than the buffer length.');
    }
    
    this._socket.write(blockDataNetBuffer.getBuffer());
  }

  //@throws DataFormatError
  protected handleBlockReceive(): void {
    if(this.currentBlockNetBuffer != null) {
      throw new DataFormatError("Error.");
    }
    
    let blockLen: number;
    let blockIndice: number;
    let isBlockLenComputed: boolean = false;
    
    let combinedPacketsLen: number = 0;
    let blockPacketsCount: number = 0;
    
    for(let i: number=0;i<this._receivedNetBuffersList.length;i++) {
      let packetNetBuffer: StreamBuffer = this._receivedNetBuffersList[i];
      
      // In case the packet contains data from a previous block, ignore that part.
      combinedPacketsLen += packetNetBuffer.getPostBoundsLength();
      blockPacketsCount++;
      
      if(!isBlockLenComputed) {
        isBlockLenComputed = true;
        
        // The bounds is before the blockLen value, unsave peek is required.
        blockLen = packetNetBuffer.peekIntUnsafe(0);
        if(blockLen < NetDataLimits.BLOCK_LEN_MIN || blockLen > NetDataLimits.BLOCK_LEN_MAX) {
          // The block exceeds the lower limit of 12 bytes or the upper limit of 256KB. A error may have occured.
          
          throw new DataFormatError("Error.");
        }
        
        blockIndice = packetNetBuffer.peekIntUnsafe(4);
        if(blockIndice != this._lastReceivedDataBlockIndice + 1) {
          throw new DataFormatError("Error.");
        }
      }
      
      if(blockLen <= combinedPacketsLen) {
        this.currentBlockNetBuffer = this.extractConfirmedReceivedBlock(blockLen);

        this._lastReceivedDataBlockIndice = blockIndice;
        
        break;
      }
    }
  }
  
  //@throws DataFormatError
  protected extractConfirmedReceivedBlock(blockLen: number): StreamBuffer {
    // Handle special case when the first packet is a single block.
    let firstPacketNetBuffer: StreamBuffer = this._receivedNetBuffersList[0];
    if(firstPacketNetBuffer.getPostBoundsLength() == blockLen) {
      this._receivedNetBuffersList.splice(0, 1);
      
      let ret: StreamBuffer = firstPacketNetBuffer;
      // Increase the bounds by 4+4 to skip over the blockLen and blockIndice values because peek was used to read them.
      ret.setBoundsLimit(ret.getBoundsLimit() + 4 + 4);
      
      return ret;
    }
    
    // Extract packets into the block buffer until the block is created.
    let ret: StreamBuffer = new StreamBuffer(Buffer.allocUnsafe(blockLen));
    
    let neededLen: number = blockLen;
    while(this._receivedNetBuffersList.length > 0) {
      let packetNetBuffer: StreamBuffer = this._receivedNetBuffersList[0];
      
      let nextWriteLen: number = Math.min(neededLen, packetNetBuffer.getPostBoundsLength());
      
      if(packetNetBuffer.getOffset() != packetNetBuffer.getBoundsLimit()) {
        throw new DataFormatError("Error.");
      }
      packetNetBuffer.setBoundsLimit(packetNetBuffer.getBoundsLimit() + nextWriteLen);
      
      ret.writeNetBuffer(packetNetBuffer, nextWriteLen);
      
      neededLen -= nextWriteLen;
      
      // No more data in this packet, remove it from the list.
      if(packetNetBuffer.getPostBoundsLength() <= 0) {
        this._receivedNetBuffersList.splice(0, 1);
      }
      
      if(neededLen < 0) {
        throw new DataFormatError("Error.");
      }
      if(neededLen == 0) {
        ret.setBoundsLimit(4 + 4);
        
        // Block data read finished.
        
        break;
      }
    }

    return ret;
  }
  
}
