package base.file;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.files.FileHandle;

/**
 * Created by Adrin on 9/2/2016.
 */
public class FileUtil {

    public static final int FILE_SYSTEM_INTERNAL = 1;
    public static final int FILE_SYSTEM_EXTERNAL = 2;
    public static final int FILE_SYSTEM_LOCAL = 3;

    public static FileHandle getFileHandler(int fileSystem, String filePath) {
        FileHandle fh;

        if(fileSystem == FILE_SYSTEM_INTERNAL) {
            fh = Gdx.files.internal(filePath);
        }else if(fileSystem == FILE_SYSTEM_EXTERNAL) {
            fh = Gdx.files.external(filePath);
        }else if(fileSystem == FILE_SYSTEM_LOCAL) {
            fh = Gdx.files.local(filePath);
        }else {
            throw new Error("AnimatedComposedStiledVisual.Error.7.");
        }

        return fh;
    }

    public static boolean copyFile(int sourceFileSystem, String sourceFilePath, int destFileSystem, String destFilePath) {
        // Delete image at externalFilePathB;
        if(!deleteFile(destFileSystem, destFilePath)) {
            return false;
        }

        // Copy the file.
        FileHandle from = getFileHandler(sourceFileSystem, sourceFilePath);

        FileHandle to = getFileHandler(destFileSystem, destFilePath);

        try {
            from.copyTo(to);
        }catch (Exception e) {
            // Screenshot save hhtpRequestFailed. Reason may be insufficient space.

            return false;
        }

        return true;
    }

    public static boolean deleteFile(int fileSystem, String filePath) {
        // Delete image at externalFilePathB;
        FileHandle fh = getFileHandler(fileSystem, filePath);

        try {
            int triesCount = 0;
            while(fh.exists()) {
                fh.delete();

                triesCount++;
                if(triesCount >= 20) {
                    return false;
                }
            }
        }catch (Exception e) {
            // Screenshot save hhtpRequestFailed. Reason may be insufficient space.

            return false;
        }

        return true;
    }

}
