import { addListener } from "cluster";

export class Base64 {
    
    public static encode(srcBuf: Buffer): string {
        let base64Value: string = srcBuf.toString("base64");

        return base64Value;
    }
    
    public static decode(srcBase64: string): Buffer {
        let bytesValue: Buffer = Buffer.from(srcBase64, "base64");

        return bytesValue;
    }
    
  }
  