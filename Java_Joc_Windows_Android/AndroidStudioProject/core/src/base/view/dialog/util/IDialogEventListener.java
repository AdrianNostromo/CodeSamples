package base.view.dialog.util;

import base.menu.util.DynamicTextButtonConfig;
import base.view.dialog.DialogView;

/**
 * Created by Adrin on 6/23/2016.
 */
public interface IDialogEventListener {

    void dynamicButtonActivated(DialogView view, DynamicTextButtonConfig dynamicButtonConfig);
    void closeRequested(DialogView view);

}
