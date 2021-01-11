import { IApp } from "base/app/IApp";
import { ISquareHeadsManager } from "squareheads/ISquareHeadsManager";

export interface IAppLocal extends IApp {

    getSquareHeads(): ISquareHeadsManager;
    
}
