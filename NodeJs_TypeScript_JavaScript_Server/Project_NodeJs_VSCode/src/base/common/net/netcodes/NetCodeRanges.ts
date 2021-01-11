export class NetCodeRanges {

  public static readonly SC_UnvalidatedServerComponentToUnvalidatedClientComponent: number = 101000;
  public static readonly CS_UnvalidatedClientComponentToUnvalidatedServerComponent: number = 101500;

  public static readonly SC_ServerComponentToClientComponent: number = 102000;
  public static readonly CS_ClientComponentToServerComponent: number = 102500;

  public static readonly ComponentToComponent: number = 103000;

  public static readonly SC_ServerManagerToClientComponent: number = 104000;
  public static readonly CS_ClientComponentToServerManager: number = 104500;

  public static readonly SC_ServerManager_To_NodeController: number = 105000;
  public static readonly CS_NodeController_To_ServerManager: number = 105500;

  public static readonly SC_AssetUploadInteractor_To_EntryPointInteractor: number = 106000;
  public static readonly CS_EntryPointInteractor_To_AssetUploadInteractor: number = 106500;

  public static readonly SC_AccountsDbManager_To_AccountsManager: number = 107000;
  public static readonly CS_AccountsManager_To_AccountsDbManager: number = 107500;

  public static readonly SC_AccountsManager_To_AssetUploadInteractor: number = 108000;
  public static readonly CS_AssetUploadInteractor_To_AccountsManager: number = 108500;

  public static readonly SC_AssetsManager_To_AssetUploadInteractor: number = 109000;
  public static readonly CS_AssetUploadInteractor_To_AssetsManager: number = 109500;

  public static readonly SC_AssetsDbManager_To_AssetsManager: number = 110000;
  public static readonly CS_AssetsManager_To_AssetsDbManager: number = 110500;

  public static readonly SC_GMatchHistoryDbManager_To_GMatchHistoryManager: number = 111000;
  public static readonly CS_GMatchHistoryManager_To_GMatchHistoryDbManager: number = 111500;

  public static readonly SC_GMatchHistoryManager_To_GMatchStatusInteractor: number = 112000;
  public static readonly CS_GMatchStatusInteractor_To_GMatchHistoryManager: number = 112500;

  public static readonly SC_GMatchHistoryManager_To_GMatchManager: number = 113000;
  public static readonly CS_GMatchManager_To_GMatchHistoryManager: number = 113500;

  public static readonly SC_GMatchFinderInteractor_To_EntryPointInteractor: number = 114000;
  public static readonly CS_EntryPointInteractor_To_GMatchFinderInteractor: number = 114500;

  public static readonly SC_GMatchStatusInteractor_To_EntryPointInteractor: number = 115000;
  public static readonly CS_EntryPointInteractor_To_GMatchStatusInteractor: number = 115500;

  public static readonly SC_AccountsManager_To_GMatchFinderInteractor: number = 116000;
  public static readonly CS_GMatchFinderInteractor_To_AccountsManager: number = 116500;

  public static readonly SC_GMatchManager_To_AssetUploadInteractor: number = 117000;
  public static readonly CS_AssetUploadInteractor_To_GMatchManager: number = 117500;

  public static readonly SC_GMatchManager_To_GMatchInstanceInteractor: number = 118000;
  public static readonly CS_GMatchInstanceInteractor_To_GMatchManager: number = 118500;

  public static readonly SC_GMatchManager_To_GMatchFinderInteractor: number = 119000;
  public static readonly CS_GMatchFinderInteractor_To_GMatchManager: number = 119500;

}
