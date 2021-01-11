package base.timer.util;

public class TimerItem {

    public base.timer.ITimerManager timerManager;

    public float currentDelayS;

    public TimerEventListener eventListener;

    public TimerItem(base.timer.ITimerManager timerManager, float currentDelayS, TimerEventListener eventListener) {
        super();

        this.timerManager = timerManager;

        this.currentDelayS = currentDelayS;

        this.eventListener = eventListener;
    }

    public void stop() {
        if(timerManager != null) {
            timerManager.stopTimerRequested(this);
        }
    }

}
