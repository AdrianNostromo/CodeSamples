import { ClientAccountsManagerBase } from "./ClientAccountsManagerBase";

import { IAppLocal } from "./../app/IAppLocal";

import { CustomClientAccInfo_SocialUserId_Nickname } from "./util/CustomClientAccInfo_SocialUserId_Nickname";
import { DbAccountEntry } from "squareheads/clientsdatastore/util/DbAccountEntry";

export class ClientAccountsManager extends ClientAccountsManagerBase {

  public constructor(app: IAppLocal) {
    super(app);
    
    //void
  }

  public createNewSquareHeadsAccount(): number {
    let accEntry: DbAccountEntry = this.getApp().getSquareHeads().getClientsDatastore().insertAccount("DooD");
    
    return accEntry.accountId;
  }

  public getUserCustomDataFromAccId_socialUserId_nickanme(accountId: number): CustomClientAccInfo_SocialUserId_Nickname {
    let accEntry: DbAccountEntry = this.getApp().getSquareHeads().getClientsDatastore().selectUserWithAccountId(accountId);
    if(accEntry == null) {
      return null;
    }

    let ret: CustomClientAccInfo_SocialUserId_Nickname = new CustomClientAccInfo_SocialUserId_Nickname(
      accEntry.socialUserId,
      accEntry.nickname
    );

    return ret;
  }
  
}
