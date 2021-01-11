import { TcpDataBlocksConnection } from './TcpDataBlocksConnection';

import * as net from "net";
import { LogicError } from "base/exception/LogicError";
import { DataFormatError } from 'base/net/exception/DataFormatError';
import { NetDataLimits } from 'base/common/net/NetDataLimits';
import { StreamBuffer } from "base/buffer/StreamBuffer";

export class TcpRequestBlocksConnection extends TcpDataBlocksConnection {

  public constructor(socket: net.Socket) {
    super(socket);
    
    //void
  }
  
  //@throws DataFormatError, Error
  public popNextMsg(): StreamBuffer {
    if(this.currentBlockNetBuffer != null) {
      let msgBuff: StreamBuffer = this._getNextMessageFromCurrentBlockBuffer(false);
      if(msgBuff != null) {
        return msgBuff;
      }
    }
    
    this.handleBlockReceive();
    if(this.currentBlockNetBuffer != null) {
      let msgBuff: StreamBuffer = this._getNextMessageFromCurrentBlockBuffer(true);
      if(msgBuff == null) {
        throw new LogicError('Logic error. This implies a empty block received.');
      }
      
      return msgBuff;
    }
    
    return null;
  }
  
  private _getNextMessageFromCurrentBlockBuffer(isMsgMandatory: boolean): StreamBuffer {
    if(this.currentBlockNetBuffer == null) {
      return null;
    }
    
    // Check if buffer has any unprocessed messages.
    if(this.currentBlockNetBuffer.getBoundsLimit() >= this.currentBlockNetBuffer.getLength()) {
      // End of block data reached.
      
      if(isMsgMandatory) {
        throw new DataFormatError("Error.");
      }
      
      this.currentBlockNetBuffer = null;
      
      return null;
    }
    
    // There are more messages. Validate the msg size to make sure the msgLen value is valid.
    this.currentBlockNetBuffer.setOffset(this.currentBlockNetBuffer.getBoundsLimit());
    let msgLen: number = this.currentBlockNetBuffer.peekIntUnsafe(0);
    if(msgLen == 0) {
      // The end of the block zero padding occured. There is no more messages in this block.
      
      if(isMsgMandatory) {
        throw new DataFormatError("Error.");
      }
      
      this.currentBlockNetBuffer = null;
      
      return null;
    }
    
    if(msgLen < NetDataLimits.MSG_LEN_MIN || msgLen > NetDataLimits.MSG_LEN_MAX) {
      // The msg length exceeds the lower limit of 8 bytes or the upper limit of 256KB. A error may have occured.
      
      throw new DataFormatError("Error.");
    }
    
    if(this.currentBlockNetBuffer.getBoundsLimit() + msgLen > this.currentBlockNetBuffer.getLength()) {
      // There is not enough data for this message, the msgLen value is not correct.
      throw new DataFormatError("Error.");
    }
    
    this.currentBlockNetBuffer.setBoundsLimit(this.currentBlockNetBuffer.getBoundsLimit() + msgLen);
    
    return this.currentBlockNetBuffer;
  }
  
}
