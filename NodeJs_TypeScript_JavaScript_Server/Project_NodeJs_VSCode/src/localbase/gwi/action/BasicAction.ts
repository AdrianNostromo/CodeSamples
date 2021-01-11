import { BasicPlayer } from "../util/BasicPlayer";

export abstract class BasicAction {

  public type: number;

  public pl: BasicPlayer = null;

  public isProcessed: boolean = false;

  public constructor(type: number) {
    this.type = type;
  }

}
