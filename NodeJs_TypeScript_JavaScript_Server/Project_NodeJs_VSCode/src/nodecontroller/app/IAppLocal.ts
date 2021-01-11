import { IApp } from "base/app/IApp";
import { NodeController } from "nodecontroller/NodeController";

export interface IAppLocal extends IApp {

    getNodeController(): NodeController;
    
}
