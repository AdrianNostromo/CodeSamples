"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class BasicNetcutsFactory {
    constructor() {
        //void
    }
    initEntries(netcutConfigsList) {
        this.netcutConfigsList = netcutConfigsList;
    }
    getNetcutConfig(connectionType, componentType) {
        for (let i = 0; i < this.netcutConfigsList.length; i++) {
            let entry = this.netcutConfigsList[i];
            if (entry != null
                && entry.connectionType == connectionType
                && entry.componentType == componentType) {
                return entry;
            }
        }
        return null;
    }
}
BasicNetcutsFactory.CONNECTION_TYPE_SERVER = 1;
BasicNetcutsFactory.CONNECTION_TYPE_CLIENT = 2;
exports.BasicNetcutsFactory = BasicNetcutsFactory;
//# sourceMappingURL=BasicNetcutsFactory.js.map