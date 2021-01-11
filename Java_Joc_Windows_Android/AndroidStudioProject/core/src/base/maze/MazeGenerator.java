package base.maze;

import java.util.Random;

import base.maze.algorithm.RecursiveDivision;
import base.maze.path.LongestMazePathGenerator;
import base.maze.util.MazeUtil;
import base.util.Point2DInt;

/**
 * Created by adrin on 3/30/17.
 */

public class MazeGenerator {
    
    private int rows;
    private int cols;
    
    private Random rand;
    
    public int[][]maze = null;
    
    private LongestMazePathGenerator longestPathGen = null;
    
    public MazeGenerator(int rows, int cols, Random rand) {
        super();
    
        this.rows = rows;
        this.cols = cols;
    
        this.rand = rand;
    }
    
    public void generateMaze() {
        RecursiveDivision mazeGen = new RecursiveDivision(rows, cols, rand);
        mazeGen.makeMaze();
        
        maze = mazeGen.getMaze();
    
        // Make sure the maze edges don't have the original entrance and exit.
        for(int i=0;i<maze.length;i++) {
            maze[i][0] = MazeUtil.MAZE_WALL;
            maze[i][maze[i].length - 1] = MazeUtil.MAZE_WALL;
        }
        for(int i=0;i<maze[0].length;i++) {
            maze[0][i] = MazeUtil.MAZE_WALL;
            maze[maze.length - 1][i] = MazeUtil.MAZE_WALL;
        }
    }
    
    public Point2DInt[] generateLongestPath() {
        longestPathGen = new LongestMazePathGenerator(maze);
        
        if(!longestPathGen.computeLongestPath()) {
            return null;
        }
        
        return longestPathGen.getLongestPath();
    }
    
    public int[][] getMaze() {
        return maze;
    }
    
    public void printMaze() {
        String s = "";
        
        for(int i=0; i<maze.length; i++){
            String ss = "";
            for(int j=0; j<maze[i].length; j++){
                if(maze[i][j] == MazeUtil.MAZE_PATH) {
                    ss = ss + "#";
                }else if(maze[i][j] == MazeUtil.MAZE_WALL) {
                    ss = ss + " ";
                }else if(maze[i][j] == MazeUtil.MAZE_SOLUTION) {
                    ss = ss + "+";
                }else {
                    ss = ss + "?z";
                }
            }
            
            s = ss + "\n" + s;
        }
        
        System.out.println(s);
    }
    
}
