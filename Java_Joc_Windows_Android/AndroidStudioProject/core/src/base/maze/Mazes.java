package base.maze;

import java.util.Random;

import base.maze.algorithm.Ellers;
import base.maze.algorithm.GrowingTree;
import base.maze.algorithm.RecursiveDivision;

public class Mazes
{
        static final int ROWS = 12;
        static final int COLS = 12;

    public static void main(String [] args)
    {




        
        System.out.println("Recursive Backtracker Maze and Solution:");

        base.maze.algorithm.Backtracker recBack = new base.maze.algorithm.Backtracker(ROWS,COLS);
        recBack.makeMaze();
        recBack.printMaze();

        System.out.println();
/*
        Solver recBackSol = new Solver(recBack.getMaze() );
        recBackSol.solveMaze();
        recBackSol.printSolution();
*/
        System.out.println("\n");
       



        System.out.println("Recursive Division Maze and Solution:");

        base.maze.algorithm.RecursiveDivision recDiv = new RecursiveDivision(ROWS,COLS, new Random(1));
        recDiv.makeMaze();
        recDiv.printMaze();

        System.out.println();

        Solver recDivSol = new Solver(recDiv.getMaze() );
        recDivSol.solveMaze();
        recDivSol.printSolution();

        System.out.println("\n");



        
        System.out.println("Eller's Algorithm Maze and Solution:");

        base.maze.algorithm.Ellers ell = new Ellers(ROWS,COLS);
        ell.makeMaze();
        ell.printMaze();
        
        System.out.println();
/*
        Solver ellSol = new Solver(ell.getMaze());
        ellSol.solveMaze();
        ellSol.printSolution();
*/
        System.out.println("\n");



        System.out.println("Growing Tree Algorithm Maze and Solution:");

        base.maze.algorithm.GrowingTree gTree = new GrowingTree(ROWS, COLS);
        gTree.makeMaze();
        gTree.printMaze();

        System.out.println();
/*
        Solver gTreeSol = new Solver(gTree.getMaze());
        gTreeSol.solveMaze();
        gTreeSol.printSolution();
*/
        System.out.println("\n");
        
    }
}
