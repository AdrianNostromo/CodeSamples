package base.maze.path;

import java.util.Vector;

import base.maze.util.MazeUtil;
import base.util.Point2DInt;

/**
 * Created by adrin on 3/30/17.
 */

public class LongestMazePathGenerator {
    
    private static final int MAZE_TILE_PROCESSED = -1;
    
    private static class Node {
        public int layerIndex;
        public int gX;
        public int gY;
        
        public Node p;
        public Vector<Node> cList = null;
        
        public Node(int layerIndex, int gX, int gY, Node p) {
            super();
            
            this.layerIndex = layerIndex;
            this.gX = gX;
            this.gY = gY;
            
            this.p = p;
        }
        
    }
    
    private int[][] inputMaze;
    
    private Node mazeTreeRoot = null;
    
    private int[][] maze = null;
    private Point2DInt[] longsetPath = null;
    
    public LongestMazePathGenerator(int[][] inputMaze) {
        super();
        
        this.inputMaze = inputMaze;
    }
    
    public boolean computeLongestPath() {
        maze = MazeUtil.clone2DIntMaze(inputMaze);
    
        Point2DInt initialMazeTreePoint = findAnyMazePathPoint(maze);
        if(initialMazeTreePoint == null) {
            return false;
        }
    
        mazeTreeRoot = new Node(0, initialMazeTreePoint.x, initialMazeTreePoint.y, null);
        maze[mazeTreeRoot.gY][mazeTreeRoot.gX] = MAZE_TILE_PROCESSED;
        
        Vector<Node> leafNodes = new Vector<Node>();
        
        processNodeUnders(mazeTreeRoot, maze, leafNodes);
        
        Vector<Node> longestNodesPath = computeLongestLeafsPath(leafNodes);
    
        longsetPath = new Point2DInt[longestNodesPath.size()];
        for(int i=0;i<longestNodesPath.size();i++) {
            Node node = longestNodesPath.get(i);
            
            longsetPath[i] = new Point2DInt(node.gX, node.gY);
        }
        
        return true;
    }
    
    private static void processNodeUnders(Node node, int[][] maze, Vector<Node> leafNodes) {
        for(int i=0;i<MazeUtil.connectedMazeGridOffsets.length;i++) {
            Point2DInt gridOffset = MazeUtil.connectedMazeGridOffsets[i];
            
            int gX = node.gX + gridOffset.x;
            int gY = node.gY + gridOffset.y;
    
            boolean isTileInMaze = MazeUtil.isTileOnMazeYX(maze, gX, gY);
            if(isTileInMaze && maze[gY][gX] == MazeUtil.MAZE_PATH) {
                maze[gY][gX] = MAZE_TILE_PROCESSED;
    
                Node underNode = new Node(node.layerIndex + 1, gX, gY, node);
                if (node.cList == null) {
                    node.cList = new Vector<Node>();
                }
                node.cList.add(underNode);
    
                processNodeUnders(underNode, maze, leafNodes);
            }
        }
        
        if(node.cList == null) {
            leafNodes.add(node);
        }else if(node.p == null && node.cList.size() == 1) {
            leafNodes.add(node);
        }
    }
    
    private static Vector<Node> computeLongestLeafsPath(Vector<Node> leafNodes) {
        Vector<Node> longestPath = null;
        
        for(int i=0;i<leafNodes.size();i++) {
            Node nodeA = leafNodes.get(i);
    
            for(int j=i+1;j<leafNodes.size();j++) {
                Node nodeB = leafNodes.get(j);
                
                Vector<Node> currentPath = getLeafNodesPath(nodeA, nodeB);
                
                if(longestPath == null || currentPath.size() > longestPath.size()) {
                    longestPath = currentPath;
                }
            }
        }
        
        return longestPath;
    }
    
    private static Vector<Node> getLeafNodesPath(Node nodeA, Node nodeB) {
        Vector<Node> pathFromA = new Vector<Node>();
        Vector<Node> pathFromB = new Vector<Node>();
        
        Node topNodeA = nodeA;
        Node topNodeB = nodeB;
    
        pathFromA.add(topNodeA);
        pathFromB.add(topNodeB);
        
        // Normalize the 2 paths to the same layer level;
        while(topNodeA.layerIndex > topNodeB.layerIndex) {
            topNodeA = topNodeA.p;
            pathFromA.add(topNodeA);
        }
        while(topNodeB.layerIndex > topNodeA.layerIndex) {
            topNodeB = topNodeB.p;
            pathFromB.add(topNodeB);
        }
    
        // Go up on the tree with both paths until the same point is reached;
        while(topNodeA != topNodeB) {
            topNodeA = topNodeA.p;
            pathFromA.add(topNodeA);
            
            topNodeB = topNodeB.p;
            pathFromB.add(topNodeB);
        }
        
        Vector<Node> ret = new Vector<Node>();
        for(int i=0;i<pathFromA.size();i++) {
            ret.add(pathFromA.get(i));
        }
        // Start from a offset of (-1) because the last entry from pathFromB is the same as the last entry from pathFromA and has already been added to the list.
        for(int i=pathFromB.size()-1 - 1;i>=0;i--) {
            ret.add(pathFromB.get(i));
        }
        
        return ret;
    }
    
    private static Point2DInt findAnyMazePathPoint(int[][] maze) {
        for(int i=0; i<maze.length; i++) {
            for (int j = 0; j < maze[i].length; j++) {
                if(maze[i][j] == MazeUtil.MAZE_PATH) {
                    return new Point2DInt(j, i);
                }
            }
        }
        
        return null;
    }
    
    public int[][] getMaze() {
        return maze;
    }
    
    public Point2DInt[] getLongestPath() {
        return longsetPath;
    }
    
}
