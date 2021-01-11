export class BooleanUtil {

  public static is(bitMap: number, bitIndex: number): boolean {
    return (bitMap & (1 << bitIndex)) != 0;
  }

  public static set(bitMap: number, bitIndex: number): number {
    return bitMap | (1 << bitIndex);
  }

  public static clear(bitMap: number, bitIndex: number): number {
    return bitMap & (~(1 << bitIndex));
  }

  public static writeBitToBitMap(
    destBitMap: Buffer, destByteIndex: number, destBitIndex: number, 
    srcValue: number, srcBitIndex: number
  ): void {
    // Extract the byte.
    let targetByte: number = destBitMap.readInt8(destByteIndex);
    
    // Modify the byte.
    if(BooleanUtil.is(srcValue, srcBitIndex)) {
      targetByte = BooleanUtil.set(targetByte, destBitIndex);
    }else {
      targetByte = BooleanUtil.clear(targetByte, destBitIndex);
    }

    // Write back the byte.
    destBitMap.writeUInt8(targetByte, destByteIndex);
  }

}
