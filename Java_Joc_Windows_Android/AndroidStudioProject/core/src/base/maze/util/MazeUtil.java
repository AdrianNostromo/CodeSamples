package base.maze.util;

import base.util.Point2DInt;

/**
 * Created by adrin on 3/30/17.
 */

public class MazeUtil {
    
    public static final int MAZE_WALL = 0;
    public static final int MAZE_PATH = 1;
    public static final int MAZE_SOLUTION = 2;
    
    public static final Point2DInt[] connectedMazeGridOffsets = new Point2DInt[]{
            new Point2DInt(-1, 0),
            new Point2DInt(1, 0),
            new Point2DInt(0, -1),
            new Point2DInt(0, 1)
    };
    
    public static int[][] clone2DIntMaze(int[][] arrIn) {
        int [][] ret = new int[arrIn.length][];
        for(int i = 0; i < arrIn.length; i++) {
            ret[i] = arrIn[i].clone();
        }
        
        return ret;
    }
    
    public static boolean isTileOnMazeYX(int[][] mazeYX, int gX, int gY) {
        if(gX >= 0
                && gY >= 0
                && gY < mazeYX.length
                && gX < mazeYX[gY].length)
        {
            return true;
        }
        
        return false;
    }
    
    public static boolean isPathableTileOnMazeYX(int[][] mazeYX, int gX, int gY) {
        if(!isTileOnMazeYX(mazeYX, gX, gY)) {
            return false;
        }
        
        if(mazeYX[gY][gX] == MAZE_PATH) {
            return true;
        }
        
        return false;
    }
    
    public static float getAngDFromMazeDIrection(int dirX, int dirY) {
        float ret;
        
        if(dirX == 0 && dirY > 0) {
            ret = 90f;
        }else if(dirX == 0 && dirY < 0) {
            ret = -90f;
        }else if(dirX < 0 && dirY == 0) {
            ret = 180f;
        }else {
            ret = 0f;
        }
        
        return ret;
    }
    
}
