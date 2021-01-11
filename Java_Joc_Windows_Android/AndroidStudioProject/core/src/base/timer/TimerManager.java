package base.timer;

import java.util.Vector;

public class TimerManager implements ITimerManager {

    private Vector<base.timer.util.TimerItem> timersList = new Vector<base.timer.util.TimerItem>();

    public TimerManager() {
        super();

        //void
    }

    public void create() {
        //void
    }

    public void appLoop(float deltaS, int deltaMS) {
        for(int i=timersList.size()-1;i>=0;i--) {
            base.timer.util.TimerItem timerItem = timersList.get(i);
            if (timerItem != null) {
                timerItem.currentDelayS = Math.max(0, timerItem.currentDelayS - deltaS);
                if(timerItem.currentDelayS <= 0) {
                    if(timerItem.eventListener != null) {
                        timerItem.eventListener.timerFinished(timerItem);
                    }

                    timerItem.stop();
                }
            }
        }
    }

    public base.timer.util.TimerItem newTimer(float delayS, base.timer.util.TimerEventListener eventListener) {
        base.timer.util.TimerItem timerItem = new base.timer.util.TimerItem(this, delayS, eventListener);

        timersList.add(timerItem);

        return timerItem;
    }

    @Override
    public void stopTimerRequested(base.timer.util.TimerItem timer) {
        timer.timerManager = null;

        timersList.remove(timer);
    }
}
