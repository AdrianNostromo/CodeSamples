package base.view.popupmenu.util;

/**
 * Created by Adrin on 6/23/2016.
 */
public class PopupInstanceData {

    public PopupConfig popupConfig;

    public PopupEventListener eventListener;

    public PopupInstanceData(PopupConfig popupConfig, PopupEventListener eventListener) {
        super();

        this.popupConfig = popupConfig;

        this.eventListener = eventListener;
    }
}
