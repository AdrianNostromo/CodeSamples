package base.view.popupmenu;

import base.view.popupmenu.util.PopupConfig;
import base.view.popupmenu.util.PopupEventListener;

/**
 * Created by Adrin on 8/3/2016.
 */
public interface IPopupMenu {

    void showPopup(PopupConfig popupConfig, PopupEventListener eventListener, boolean showDuplicates);

}
