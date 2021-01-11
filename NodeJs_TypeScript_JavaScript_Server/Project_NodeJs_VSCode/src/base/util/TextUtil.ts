export class TextUtil {

  public static countUtf8Bytes(value: string): number {
    let b: number = 0;
    let i: number = 0;
    let c: number;
    for(;c=value.charCodeAt(i++);b+=c>>11?3:c>>7?2:1);

    return b
  }

}
