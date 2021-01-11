"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const BasicAppComponent_1 = require("base/component/BasicAppComponent");
const ActiveSplitNetRequest_1 = require("base/common/splitnetrequests/util/ActiveSplitNetRequest");
class BasicSplitNetRequestsHandler extends BasicAppComponent_1.BasicAppComponent {
    constructor(app, remoteResComponentType) {
        super(app);
        this._activeRequestsList = [];
        this._splitNetIdCounter = 0;
        this.remoteResComponentType = remoteResComponentType;
    }
    getNewSplitNetActiveRequest(dataMap) {
        this._splitNetIdCounter++;
        let newEntry = new ActiveSplitNetRequest_1.ActiveSplitNetRequest(this._splitNetIdCounter, dataMap);
        this._activeRequestsList.push(newEntry);
        return newEntry;
    }
    popActiveRequest(splitNetID) {
        for (let i = 0; i < this._activeRequestsList.length; i++) {
            let activeRequestEntry = this._activeRequestsList[i];
            if (activeRequestEntry.splitNetID == splitNetID) {
                this._activeRequestsList.splice(i, 1);
                return activeRequestEntry;
            }
        }
        return null;
    }
}
exports.BasicSplitNetRequestsHandler = BasicSplitNetRequestsHandler;
//# sourceMappingURL=BasicSplitNetRequestsHandler.js.map