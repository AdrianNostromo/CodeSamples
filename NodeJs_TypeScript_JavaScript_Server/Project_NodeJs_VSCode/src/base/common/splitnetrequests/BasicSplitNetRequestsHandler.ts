import { BasicAppComponent } from "base/component/BasicAppComponent";

import { IApp } from "base/app/IApp";
import { ComponentType } from "base/common/ComponentType";

import { ActiveSplitNetRequest } from "base/common/splitnetrequests/util/ActiveSplitNetRequest";

export class BasicSplitNetRequestsHandler extends BasicAppComponent {

  public remoteResComponentType: ComponentType;
  
  private _activeRequestsList: ActiveSplitNetRequest[] = [];
  private _splitNetIdCounter: number = 0;

  public constructor(app: IApp, remoteResComponentType: ComponentType) {
    super(app);
    
    this.remoteResComponentType = remoteResComponentType;
  }
  
  public getNewSplitNetActiveRequest(dataMap: Map<string, Object>): ActiveSplitNetRequest {
    this._splitNetIdCounter++;
    
    let newEntry: ActiveSplitNetRequest = new ActiveSplitNetRequest(
      this._splitNetIdCounter, 
      dataMap
    );
    this._activeRequestsList.push(newEntry);
    
    return newEntry;
  }
  
  public popActiveRequest(splitNetID: number): ActiveSplitNetRequest {
    for(let i: number=0;i<this._activeRequestsList.length;i++) {
      let activeRequestEntry: ActiveSplitNetRequest = this._activeRequestsList[i];
      if(activeRequestEntry.splitNetID == splitNetID) {
        this._activeRequestsList.splice(i, 1);
        
        return activeRequestEntry;
      }
    }
    
    return null;
  }
  
}
