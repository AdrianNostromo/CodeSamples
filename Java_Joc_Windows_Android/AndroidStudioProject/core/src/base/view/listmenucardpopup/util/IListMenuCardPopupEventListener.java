package base.view.listmenucardpopup.util;

import base.view.listmenucardpopup.IListMenuCardPopupView;
import base.visual.List;
import base.visual.ListEntry;

/**
 * Created by Adrin on 8/30/2016.
 */
public interface IListMenuCardPopupEventListener {

    void listEntryActivated(IListMenuCardPopupView targetView, List list, ListEntry listEntry);
    void closeRequested(IListMenuCardPopupView targetView);

}
