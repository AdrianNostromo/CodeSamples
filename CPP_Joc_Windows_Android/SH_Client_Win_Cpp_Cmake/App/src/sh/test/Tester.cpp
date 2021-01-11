#include "Tester.h"

using namespace sh;

Tester::Tester(IAppSquareHeads* app)
    : super(app)
{
    //void
}

void Tester::onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event) {
    super::onLoopEvent_appLoop_general(eventListener, event);

//    float oldTimeS = timeS;
//    timeS += deltaS;

    /*if(oldTimeS <= 3.0f * 1.0f && timeS > 3.0f * 1.0f) {
        app->getLogger()->postDebug("asd: 1");

        app->getStatusView()->setIsInternet(false);
        app->getStatusView()->setIsConnectedToServer(false);

        app->getStatusView()->activateStatus(IStatusView::StatusType::ConnectToServer_inProgress, -1.0f);
    }else if(oldTimeS <= 3.0f * 2.0f && timeS > 3.0f * 2.0f) {
        app->getStatusView()->activateStatus(IStatusView::StatusType::ConnectToServer_error, -1.0f);
    }else if(oldTimeS <= 3.0f * 3.0f && timeS > 3.0f * 3.0f) {
        app->getStatusView()->setIsInternet(true);
        app->getStatusView()->setIsConnectedToServer(true);

        app->getStatusView()->activateStatus(IStatusView::StatusType::General_ok, 5.0f);
    }*/
    /*if(oldTimeS <= 3.0f * 1 && timeS > 3.0f * 1) {
        app->getLogger()->postDebug("asd: 1");

        app->getStatusView()->setIsInternet(false);
        app->getStatusView()->setIsConnectedToServer(false);

        app->getStatusView()->activateStatus(IStatusView::StatusType::General_ok, -1.0f);
    }else if(oldTimeS <= 3.0f * 2 && timeS > 3.0f * 2) {
        app->getStatusView()->activateStatus(IStatusView::StatusType::ConnectToServer_error, -1.0f);
    }*/
    /*if(oldTimeS <= 3.0f * 1.0f && timeS > 3.0f * 1.0f) {
        app->getLogger()->postDebug("asd: 1");

        app->getStatusView()->setIsInternet(false);
        app->getStatusView()->setIsConnectedToServer(false);

        app->getStatusView()->activateStatus(IStatusView::StatusType::ConnectToServer_inProgress, -1.0f);
    }else if(oldTimeS <= 3.0f * 2.0f && timeS > 3.0f * 2.0f) {
        app->getStatusView()->activateStatus(IStatusView::StatusType::ConnectToServer_error, -1.0f);
    }else if(oldTimeS <= 3.0f * 3.0f && timeS > 3.0f * 3.0f) {
        app->getStatusView()->activateStatus(IStatusView::StatusType::CheckInternet_error, -1.0f);
    }else if(oldTimeS <= 3.0f * 4.0f && timeS > 3.0f * 4.0f) {
        app->getStatusView()->setIsInternet(true);
        app->getStatusView()->setIsConnectedToServer(true);

        app->getStatusView()->activateStatus(IStatusView::StatusType::General_ok, 5.0f);
    }else if(oldTimeS <= 3.0f * 5.0f && timeS > 3.0f * 5.0f) {
        app->getStatusView()->activateStatus(IStatusView::StatusType::CheckInternet_error, -1.0f);
    }else if(oldTimeS <= 3.0f * 6.0f && timeS > 3.0f * 6.0f) {
        app->getStatusView()->setIsInternet(true);
        app->getStatusView()->setIsConnectedToServer(true);

        app->getStatusView()->activateStatus(IStatusView::StatusType::General_ok, 5.0f);
    }*/
}

Tester::~Tester() {
    //void
}
