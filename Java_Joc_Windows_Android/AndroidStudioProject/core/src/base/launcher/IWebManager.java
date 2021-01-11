package base.launcher;

// Interface for the local data handler.
public interface IWebManager {

    void openUrl(String url);
    //asd_001;// rename to openUrlWithStringsID
    void openUrlWithID(String urlID);

    void shareScreenshot(String screenshotImgPath);

}
