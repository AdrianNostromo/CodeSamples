package base.maze.algorithm;

import java.util.Random;

import base.maze.util.MazeUtil;

public class RecursiveDivision
{
    
    private Random rand;
    private Random rand2;
    
    private int rows;
    private int cols;
    int act_rows;
    int act_cols;
    
    private int[][] board;

    public RecursiveDivision(int row, int col, Random rand)
    {

        //initialize instance variables
        rows = row*2+1;
        cols = col*2+1;
        
        this.rand = rand;
        this.rand2 = rand;
        
        act_rows = row;
        act_cols = col;
        board = new int[rows][cols];



        //set the maze to empty
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                board[i][j] = MazeUtil.MAZE_PATH;
            }
        }

        //make the outter walls
        for(int i=0; i<rows; i++){
            board[i][0] = MazeUtil.MAZE_WALL;
            board[i][cols-1] = MazeUtil.MAZE_WALL;
        }

        for(int i=0; i<cols; i++){
            board[0][i] = MazeUtil.MAZE_WALL;
            board[rows-1][i] = MazeUtil.MAZE_WALL;
        }


    }

    //storefront method to make the maze
    public void makeMaze()
    {
        makeMaze(0,cols-1,0,rows-1);
        makeOpenings();
    }


    //behind the scences actual mazemaking
    private void makeMaze(int left, int right, int top, int bottom)
    {
        int width = right-left;
        int height = bottom-top;

        //makes sure there is still room to divide, then picks the best
        //direction to divide into
        if(width > 2 && height > 2){

            if(width > height)
                divideVertical(left, right, top, bottom);

            else if(height > width)
                divideHorizontal(left, right, top, bottom);

            else if(height == width){
                //Random rand = new Random();
                boolean pickOne = rand.nextBoolean();

                if(pickOne)
                    divideVertical(left, right, top, bottom);
                else
                    divideHorizontal(left, right, top, bottom);
            }
        }else if(width > 2 && height <=2){
            divideVertical(left, right, top, bottom);
        }else if(width <=2 && height > 2){
            divideHorizontal(left, right, top, bottom);
        }
    }


    private void divideVertical(int left, int right, int top, int bottom)
    {
        //Random rand = new Random();

        //find a random point to divide at
        //must be even to draw a wall there
        int divide =  left + 2 + rand.nextInt((right-left-1)/2)*2;

        //draw a line at the halfway point
        for(int i=top; i<bottom; i++){
            board[i][divide] = MazeUtil.MAZE_WALL;
        }

        //get a random odd integer between top and bottom and clear it
        int clearSpace = top + rand.nextInt((bottom-top)/2) * 2 + 1;

        board[clearSpace][divide] = MazeUtil.MAZE_PATH;

        makeMaze(left, divide, top, bottom);
        makeMaze(divide, right, top, bottom);
    }

    private void divideHorizontal(int left, int right, int top, int bottom)
    {
        //Random rand = new Random();

        //find a random point to divide at
        //must be even to draw a wall there
        int divide =  top + 2 + rand.nextInt((bottom-top-1)/2)*2;
        if(divide%2 == 1)
            divide++;

        //draw a line at the halfway point
        for(int i=left; i<right; i++){
            board[divide][i] = MazeUtil.MAZE_WALL;
        }

        //get a random odd integer between left and right and clear it
        int clearSpace = left + rand.nextInt((right-left)/2) * 2 + 1;

        board[divide][clearSpace] = MazeUtil.MAZE_PATH;

        //recur for both parts of the newly split section
        makeMaze(left, right, top, divide);
        makeMaze(left, right, divide, bottom);
    }

    public void makeOpenings(){

        //Random rand = new Random(); //two different random number generators
        //Random rand2 = new Random();//just in case

        //a random location for the entrance and exit
        int entrance_row = rand.nextInt(act_rows-1) * 2 +1;
        int exit_row = rand2.nextInt(act_rows-1) * 2 +1;

        //clear the location
        board[entrance_row][0] = MazeUtil.MAZE_PATH;
        board[exit_row][cols-1] = MazeUtil.MAZE_PATH;

    }

    public void printMaze()
    {
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols; j++){
                System.out.print(board[i][j]);
            }
            System.out.println();
        }
    }

    public int getRows() {
        return rows;
    }

    public int getCols() {
        return cols;
    }
    
    public int[][] getMaze()
    {
        return board;
    }

}



