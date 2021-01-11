#include "MSV_PlayGamesStatus.h"

using namespace sh;

//asdA1;
//private PlayGamesEventListener playGamesEventListener = new PlayGamesEventListener() {
//    @Override
//    public void playGamesStatusChanged(PlayGamesStatus status, PlayGamesStatus oldPlayGamesStatus) {
//        super.playGamesStatusChanged(status, oldPlayGamesStatus);
//
//        setPlayGamesStatus(status);
//    }
//
//    @Override
//    public void playerIconLoaded(ConnectedPlayerData connectedPlayerData) {
//        super.playerIconLoaded(connectedPlayerData);
//
//        if(playGamesStatus == null || playGamesStatus != PlayGamesStatus.LOGGED_IN) {
//            return ;
//        }
//
//        onPlayerIconLoaded();
//    }
//
//    @Override
//    public void playerCanceledLoginAccountSelection() {
//        super.playerCanceledLoginAccountSelection();
//
//        // Show a popup. "Connection cancelled."
//        showConnectCancelledInfoDialog();
//    }
//};
//
//protected IAppEcosystem appEcosystem = null;
//
//private PlayGamesStatus playGamesStatus = null;

MSV_PlayGamesStatus::MSV_PlayGamesStatus(
    IApp* app,
    ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
    worldGame3D::IAppGame* appGame, IAppUtils* appUtils)
    : super(app, viewItemConfigs, viewEaseDurationsSList, appGame, appUtils)
{
    //void
}

//asdA1;
//public void initExtraData(final IAppEcosystem appEcosystem) {
//    this.appEcosystem = appEcosystem;
//}
//
//protected final PlayGamesStatus peekPlayGamesStatus() {
//    return playGamesStatus;
//}
//
//private void setPlayGamesStatus(PlayGamesStatus playGamesStatus) {
//    if (this.playGamesStatus == playGamesStatus) {
//        return;
//    }
//
//    this.playGamesStatus = playGamesStatus;
//
//    onPlayGamesStatusChanged();
//
//    // Call this and not in the change function because extenders of the change function may also require this.
//    updateShowStateIfChanged(true, null);
//}
//
//protected void onPlayGamesStatusChanged() {
//    //void
//}
//
//protected void onPlayerIconLoaded() {
//    //void
//}
//
//@Override
//protected void onSelectedStateChanged() {
//    super.onSelectedStateChanged();
//
//    if (!isActive()) {
//        //void
//    } else {
//        playGamesStatus = appEcosystem.getPlayGamesManager().getPlayGamesStatus();
//    }
//}
//
//@Override
//protected void isOnDisplayListChanged() {
//    super.isOnDisplayListChanged();
//
//    if(!isVisible()) {
//        appEcosystem.getPlayGamesManager().removeEventsListener(playGamesEventListener);
//    }else {
//        appEcosystem.getPlayGamesManager().addEventsListener(playGamesEventListener);
//    }
//}
//
//private final IDialogEventListener dialogEventListener = new IDialogEventListener() {
//    @Override
//    public void dynamicButtonActivated(DialogView view, DynamicTextButtonConfig dynamicButtonConfig) {
//        // The dialog has no buttons.
//
//        throw new LogicError("Err.1.");
//    }
//
//    @Override
//    public void closeRequested(DialogView view) {
//        view.hide(true, null);
//    }
//
//    @Override
//    public void contentAreaTouched(DialogView view) {
//        view.hide(true, null);
//    }
//};
//
//private void showConnectCancelledInfoDialog() {
//    DialogView dialogView = appUtils.getNewDisposableDialogView();
//    dialogView.show(
//        true, null,
//        new DialogConfig(
//            AppStyles.uiIncrementSizeDPM * 5f, "Connect cancelled", 1,
//            new DynamicTextItems("Disabling online features.", 1),
//            false, null
//        ),
//        dialogEventListener
//    );
//}
//
//@Override
//protected String computeStateFlags(String baseState) {
//    String state = super.computeStateFlags(baseState);
//
//    if(playGamesStatus == PlayGamesStatus.OFFLINE) {
//        state += "Offline_";
//    }else if(playGamesStatus == PlayGamesStatus.LOGGED_IN) {
//        state += "LoggedIn_";
//    }else if(playGamesStatus == PlayGamesStatus.CONNECTING) {
//        state += "Connecting_";
//    }else if(playGamesStatus == null) {
//        // the value is null during creation (before show request).
//    }else {
//        throw new LogicError("Error. 1.");
//    }
//
//    return state;
//}

MSV_PlayGamesStatus::~MSV_PlayGamesStatus() {
    //void
}
