import { ModuleBase } from "./ModuleBase";
import { BasicWorldEntity } from "./../BasicWorldEntity";
import { StreamBuffer } from "base/buffer/StreamBuffer";

export abstract class ModuleSnapshotDelta extends ModuleBase {

  public static readonly FLAGS = class {
    public static readonly LAST_USED_BIT_INDEX: number = 0 + BasicWorldEntity.FLAGS.LAST_USED_BIT_INDEX;
  };

  public constructor(moduleId: string, entity: BasicWorldEntity, usesGameLoop: boolean) {
    super(moduleId, entity, usesGameLoop);
    
    //void
  }
  
  public computeByteSize(flagsBitMap: number): number {
    let len: number = 0;

    return 0;
  }

  public writeToBuffer(buff: StreamBuffer, flagsBitMap: number): void {
    //void
  }

  public computeFullSyncFlagsBitMap(flagsBitMap: number): number {
    return flagsBitMap;
  }

  public syncFlagsCleared(): void {
    //void
  }

}
