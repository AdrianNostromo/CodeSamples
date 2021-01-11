package base.view.listmenucardpopup;

import base.menu.util.IMenuAnimationListener;
import base.view.listmenucardpopup.util.IListMenuCardPopupEventListener;
import base.view.listmenucardpopup.util.ListMenuCardPopupConfig;

/**
 * Created by Adrin on 8/30/2016.
 */
public interface IListMenuCardPopupView {

    void show(boolean doAnimation, IMenuAnimationListener menuAnimationListener, ListMenuCardPopupConfig viewConfig, IListMenuCardPopupEventListener viewEventListener);
    void hideAndDispose(boolean doAnimation);

}
