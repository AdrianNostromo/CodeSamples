"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ClientAccountsManagerBase_1 = require("./ClientAccountsManagerBase");
const CustomClientAccInfo_SocialUserId_Nickname_1 = require("./util/CustomClientAccInfo_SocialUserId_Nickname");
class ClientAccountsManager extends ClientAccountsManagerBase_1.ClientAccountsManagerBase {
    constructor(app) {
        super(app);
        //void
    }
    createNewSquareHeadsAccount() {
        let accEntry = this.getApp().getSquareHeads().getClientsDatastore().insertAccount("DooD");
        return accEntry.accountId;
    }
    getUserCustomDataFromAccId_socialUserId_nickanme(accountId) {
        let accEntry = this.getApp().getSquareHeads().getClientsDatastore().selectUserWithAccountId(accountId);
        if (accEntry == null) {
            return null;
        }
        let ret = new CustomClientAccInfo_SocialUserId_Nickname_1.CustomClientAccInfo_SocialUserId_Nickname(accEntry.socialUserId, accEntry.nickname);
        return ret;
    }
}
exports.ClientAccountsManager = ClientAccountsManager;
//# sourceMappingURL=ClientAccountsManager.js.map