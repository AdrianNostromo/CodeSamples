import { NetDataLimits } from "./NetDataLimits";
import { LogicError } from "base/exception/LogicError";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export class NetMsgsBlock extends StreamBuffer {

  private _isMsgWriteInProgress: boolean = false;
  private _currentMsgOffsetStart: number = -1;
  
  private _isLockedForSend: boolean = false;
  
  private _isBlockInitialized: boolean = false;

  public constructor(msgsCount: number, msgDataLen: number) {
    super(Buffer.allocUnsafe(4 + 4 + msgDataLen + 4 * msgsCount));
    
    //void
  }
  
  public startMsgWrite(): NetMsgsBlock {
    if(!this._isBlockInitialized) {
      this._isBlockInitialized = true;
      
      // Skip the block len and block index values.
      this.skipInt();
      this.skipInt();
    }
    
    if(this._isMsgWriteInProgress) {
      throw new LogicError('Logic error.');
    }
    
    this._isMsgWriteInProgress = true;
    this._currentMsgOffsetStart = this.getOffset();
    
    this.skipInt();

    return this;
  }
  
  public endMsgWrite(): NetMsgsBlock {
    if(!this._isMsgWriteInProgress) {
      throw new LogicError('Logic error.');
    }
    
    this._isMsgWriteInProgress = false;
    
    // Write the msg length.
    let msgEndOffset: number = this.getOffset();
    let msgLength: number = msgEndOffset - this._currentMsgOffsetStart;
    
    this.setOffset(this._currentMsgOffsetStart);
    this.writeInt(msgLength);
    
    //Check if the msg type is valid.
    let msgType: number = this.peekInt();
    if(msgType <= 0) {
      throw new LogicError('Logic error.');
    }
    
    // Finalize
    this.setOffset(msgEndOffset);

    return this;
  }
  
  public lockForSend(blockIndice: number): void {
    if(!this._isBlockInitialized || this._isMsgWriteInProgress || this._isLockedForSend) {
      throw new LogicError('Logic error.');
    }
    if(this._isMsgWriteInProgress || this.length < NetDataLimits.BLOCK_LEN_MIN || this.getOffset() != this.length || this.getBoundsLimit() != this.length) {
      throw new LogicError('Logic error.');
    }
    
    // Write the block length.
    this.setOffset(0);
    this.writeInt(this.length);
    
    this.writeInt(blockIndice);
    
    // Finalize
    this.setOffset(this.length);
    
    this._isLockedForSend = true;
  }
  
  public getIsLockedForSend(): boolean {
    return this._isLockedForSend;
  }
  
  public writeInt(value: number): NetMsgsBlock {
    super.writeInt(value);

    return this;
  }
  
  public writeBuffer(srcBuffer: Buffer): NetMsgsBlock {
    super.writeBuffer(srcBuffer);

    return this;
  }
  
  public writeString(value: string): NetMsgsBlock {
    super.writeString(value);

    return this;
  }

}
