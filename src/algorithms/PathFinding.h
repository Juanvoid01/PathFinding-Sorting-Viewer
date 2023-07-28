#pragma once

#include "../actions/ActionList.h"
#include <limits.h>
#include "../objects/Grid.h"
#include <unordered_map>
#include <queue>
#include <vector>
#include <iostream>

enum class PathFindType {
	Dijkstra, AStar, DepthFirst, BreadthFirst, Bidirectional

};


class PathFinding
{
private:

    class DijkstraCalculator {

    public:
        static void calculate(Grid* grid, ActionList* actionList) {
            actionList->addAction(new NoneAction());

            Node** nodes;
            std::priority_queue<Node, std::vector<Node>, std::greater<Node>> unvisitedNodes;

            sf::Vector2i startNode = grid->getStartNode();
            sf::Vector2i endNode = grid->getEndNode();
            sf::Vector2i rightNode, leftNode, topNode, bottomNode;

            int numRows = grid->getNumRows();
            int numCols = grid->getNumCols();

            nodes = new Node * [numRows];
            for (int i = 0; i < numRows; i++) {
                nodes[i] = new Node[numCols];
            }

            nodes[startNode.x][startNode.y].distance = 0;

            sf::Vector2i currentNode = startNode;
            unvisitedNodes.emplace(Node(startNode, 0));

            while (currentNode != endNode && !unvisitedNodes.empty())
            {
                currentNode = unvisitedNodes.top().position;
                nodes[currentNode.x][currentNode.y].visited = true;
                actionList->addAction(new MarkVisitedAction(currentNode.x, currentNode.y));
                unvisitedNodes.pop();

                //update Neighbours

                rightNode.x = currentNode.x + 1;
                rightNode.y = currentNode.y;
                leftNode.x = currentNode.x - 1;
                leftNode.y = currentNode.y;
                topNode.x = currentNode.x;
                topNode.y = currentNode.y - 1;
                bottomNode.x = currentNode.x;
                bottomNode.y = currentNode.y + 1;

                updateNeighbour(nodes, grid, currentNode, rightNode, unvisitedNodes);
                updateNeighbour(nodes, grid, currentNode, leftNode, unvisitedNodes);
                updateNeighbour(nodes, grid, currentNode, topNode, unvisitedNodes);
                updateNeighbour(nodes, grid, currentNode, bottomNode, unvisitedNodes);


            }


            //construct path

            if (currentNode == endNode) {
                reconstructPath(nodes, grid, actionList);
            }


            //free memory
            for (int i = 0; i < numRows; i++) {
                delete[] nodes[i];
            }
            delete[] nodes;

            actionList->addAction(new NoneAction());
        }
    private:

        struct Node {
        public:
            bool visited;
            sf::Vector2i position;
            sf::Vector2i parent;
            int distance; //distance to startNode

            Node() {
                visited = false;
                distance = INT_MAX;
            }
            Node(sf::Vector2i pos, int dist) {
                distance = dist;
                position = pos;
                visited = false;

            }
            Node(const Node& other) {
                position = other.position;
                distance = other.distance;
                visited = other.visited;
                parent = other.parent;
            }

            bool operator ==(const Node& other) const {
                return distance == other.distance;
            }

            bool operator >(const Node& other) const {
                return distance > other.distance;
            }

            bool operator <(const Node& other) const {
                return distance < other.distance;
            }

            Node& operator =(const Node& other) {

                position = other.position;
                distance = other.distance;
                visited = other.visited;
                parent = other.parent;

                return *this;
            }
        };

        static void updateNeighbour(Node** nodes, Grid* grid, sf::Vector2i currentNode, sf::Vector2i neighbour,
            std::priority_queue<Node, std::vector<Node>, std::greater<Node>>& unvisitedNodes) {


            if (grid->isValidPosition(neighbour) && !grid->isWall(neighbour.x, neighbour.y))
            {
                int newDistance = nodes[currentNode.x][currentNode.y].distance + 1;
                int distPlusWight = grid->getWeight(neighbour.x, neighbour.y) + newDistance;
                
                if (distPlusWight < nodes[neighbour.x][neighbour.y].distance) {

                    nodes[neighbour.x][neighbour.y].parent = currentNode;
                    nodes[neighbour.x][neighbour.y].distance = distPlusWight;

                    unvisitedNodes.emplace(Node(neighbour, distPlusWight));
                }
            }
        }

        static void reconstructPath(Node** nodes, Grid* grid, ActionList* actionList) {

            sf::Vector2i startNode = grid->getStartNode();
            sf::Vector2i endNode = grid->getEndNode();

            sf::Vector2i currentNode = endNode;

            while (currentNode != startNode)
            {
                actionList->addAction(new MarkPathAction(currentNode.x, currentNode.y));
                currentNode = nodes[currentNode.x][currentNode.y].parent;
            }
            actionList->addAction(new MarkPathAction(currentNode.x, currentNode.y));


        }
       
    };

    class AStarCalculator {
    public:
        static void calculate(Grid* grid, ActionList* actionList) {
            actionList->addAction(new NoneAction());

            Node** nodes;
            std::priority_queue<Node, std::vector<Node>, std::greater<Node>> unvisitedNodes;

            sf::Vector2i startNode = grid->getStartNode();
            sf::Vector2i endNode = grid->getEndNode();
            sf::Vector2i rightNode, leftNode, topNode, bottomNode;

            int numRows = grid->getNumRows();
            int numCols = grid->getNumCols();

            nodes = new Node* [numRows];
            for (int i = 0; i < numRows; i++) {
                nodes[i] = new Node[numCols];
            }

            
            nodes[startNode.x][startNode.y].f = 0;
            nodes[startNode.x][startNode.y].g = 0;

            sf::Vector2i currentNode = startNode;
            unvisitedNodes.emplace(Node(startNode, 0));

            while (currentNode != endNode && !unvisitedNodes.empty())
            {
                currentNode = unvisitedNodes.top().position;
                nodes[currentNode.x][currentNode.y].visited = true;
                actionList->addAction(new MarkVisitedAction(currentNode.x, currentNode.y));
                unvisitedNodes.pop();

                //update Neighbours

                rightNode.x = currentNode.x + 1;
                rightNode.y = currentNode.y;
                leftNode.x = currentNode.x - 1;
                leftNode.y = currentNode.y;
                topNode.x = currentNode.x;
                topNode.y = currentNode.y - 1;
                bottomNode.x = currentNode.x;
                bottomNode.y = currentNode.y + 1;

                updateNeighbour(nodes, grid, currentNode, rightNode, unvisitedNodes);
                updateNeighbour(nodes, grid, currentNode, leftNode, unvisitedNodes);
                updateNeighbour(nodes, grid, currentNode, topNode, unvisitedNodes);
                updateNeighbour(nodes, grid, currentNode, bottomNode, unvisitedNodes);


            }


            //construct path

            if (currentNode == endNode) {
                reconstructPath(nodes, grid, actionList);
            }


            //free memory
            for (int i = 0; i < numRows; i++) {
                delete[] nodes[i];
            }
            delete[] nodes;

            actionList->addAction(new NoneAction());
        }
    private:

        static int calculateHValue(sf::Vector2i currentNode, sf::Vector2i endNode)
        {
            //manhattan distance to the endNode

            return abs(currentNode.x - endNode.x) + abs(currentNode.y - endNode.y);
        }

        struct Node {
        public:
            bool visited;
            sf::Vector2i position;
            sf::Vector2i parent;
            int h; //distance to endNode
            int g; //distance to startNode
            int f; // g + h

            Node() {
                visited = false;
                h = INT_MAX; 
                g = INT_MAX; 
                f = INT_MAX;
            }
            Node(sf::Vector2i pos, int f) {
                this->f = f;
                position = pos;
                visited = false;
                h = INT_MAX;
                g = INT_MAX;

            }
            Node(const Node& other) {
                position = other.position;
                f = other.f;
                g = other.g;
                h = other.h;
                visited = other.visited;
                parent = other.parent;
            }

            bool operator ==(const Node& other) const {
                return f == other.f;
            }

            bool operator >(const Node& other) const {
                return f > other.f;
            }

            bool operator <(const Node& other) const {
                return f < other.f;
            }

            Node& operator =(const Node& other) {

                position = other.position;
                f = other.f;
                g = other.g;
                h = other.h;
                visited = other.visited;
                parent = other.parent;

                return *this;
            }
        };

        static void updateNeighbour(Node** nodes, Grid* grid, sf::Vector2i currentNode, sf::Vector2i neighbour,
            std::priority_queue<Node, std::vector<Node>, std::greater<Node>>& unvisitedNodes) {

            if (grid->isValidPosition(neighbour) && !grid->isWall(neighbour.x, neighbour.y))
            {

                int newGValue = nodes[currentNode.x][currentNode.y].g + 1;
                int gPlusWeight = grid->getWeight(neighbour.x, neighbour.y) + newGValue;
                int newHvalue = calculateHValue(neighbour, grid->getEndNode());
                int newFvalue = newHvalue + gPlusWeight;
                if (newFvalue < nodes[neighbour.x][neighbour.y].f) {

                    nodes[neighbour.x][neighbour.y].parent = currentNode;
                    nodes[neighbour.x][neighbour.y].f = newFvalue;
                    nodes[neighbour.x][neighbour.y].g = gPlusWeight;
                    nodes[neighbour.x][neighbour.y].h = newHvalue;

                    unvisitedNodes.emplace(Node(neighbour, newFvalue));
                }
            }
        }

        static void reconstructPath(Node** nodes, Grid* grid, ActionList* actionList) {
            
           sf::Vector2i startNode = grid->getStartNode();
           sf::Vector2i endNode = grid->getEndNode();

           sf::Vector2i currentNode = endNode;

           while (currentNode != startNode)
           {
               actionList->addAction(new MarkPathAction(currentNode.x, currentNode.y));
               currentNode = nodes[currentNode.x][currentNode.y].parent;
           }
           actionList->addAction(new MarkPathAction(currentNode.x, currentNode.y));

            
        }
    };

    class DepthFirstCalculator {

    public:
        static void calculate(Grid* grid, ActionList* actionList) {

            actionList->addAction(new NoneAction());

            Node** nodes;

            int numRows = grid->getNumRows();
            int numCols = grid->getNumCols();
            sf::Vector2i startNode = grid->getStartNode();
            sf::Vector2i endNode = grid->getEndNode();
           

            nodes = new Node*[numRows];
            for (int i = 0; i < numRows; i++)
            {
                nodes[i] = new Node[numCols];

            }

            bool pathFound = false;

            DFS(nodes, grid, actionList, startNode, pathFound);


            if (pathFound == true)
            {
                reconstructPath(nodes, grid, actionList);
            }

            for (int i = 0; i < numRows; i++) {
                delete[] nodes[i];
            }
            delete[] nodes;


            actionList->addAction(new NoneAction());


        }
    private:

        struct Node {
        public:
            bool visited = false;
            sf::Vector2i parent;
            Node() {
            }
        };

        static void DFS(Node** nodes, Grid* grid, ActionList* actionList, sf::Vector2i currentNode, bool& pathFound) {

            nodes[currentNode.x][currentNode.y].visited = true;
            actionList->addAction(new MarkVisitedAction(currentNode.x, currentNode.y));
           
            if (currentNode == grid->getEndNode()) {
                pathFound = true;
            }
            else {
                sf::Vector2i rightNode, leftNode, topNode, bottomNode;

                rightNode.x = currentNode.x + 1;
                rightNode.y = currentNode.y;
                leftNode.x = currentNode.x - 1;
                leftNode.y = currentNode.y;
                topNode.x = currentNode.x;
                topNode.y = currentNode.y - 1;
                bottomNode.x = currentNode.x;
                bottomNode.y = currentNode.y + 1;


                if (grid->isValidPosition(rightNode) && !nodes[rightNode.x][rightNode.y].visited && !grid->isWall(rightNode.x, rightNode.y))
                {
                    if (!pathFound)
                    {
                        nodes[rightNode.x][rightNode.y].parent = currentNode;
                        DFS(nodes, grid, actionList, rightNode, pathFound);
                    } 
                }
                if (grid->isValidPosition(leftNode) && !nodes[leftNode.x][leftNode.y].visited && !grid->isWall(leftNode.x, leftNode.y))
                {
                    if (!pathFound)
                    {
                        nodes[leftNode.x][leftNode.y].parent = currentNode;
                        DFS(nodes, grid, actionList, leftNode, pathFound);
                    }
                }
                if (grid->isValidPosition(topNode) && !nodes[topNode.x][topNode.y].visited && !grid->isWall(topNode.x, topNode.y))
                {
                    if (!pathFound)
                    {
                        nodes[topNode.x][topNode.y].parent = currentNode;
                        DFS(nodes, grid, actionList, topNode, pathFound);
                    }
                }
                if (grid->isValidPosition(bottomNode) && !nodes[bottomNode.x][bottomNode.y].visited && !grid->isWall(bottomNode.x, bottomNode.y))
                {
                    if (!pathFound)
                    {
                        nodes[bottomNode.x][bottomNode.y].parent = currentNode;
                        DFS(nodes, grid, actionList, bottomNode, pathFound);
                    }
                }
            }


            
        }


        static void reconstructPath(Node** nodes, Grid* grid, ActionList* actionList) {

            sf::Vector2i startNode = grid->getStartNode();
            sf::Vector2i endNode = grid->getEndNode();

            sf::Vector2i currentNode = endNode;

            while (currentNode != startNode)
            {
                actionList->addAction(new MarkPathAction(currentNode.x, currentNode.y));
                currentNode = nodes[currentNode.x][currentNode.y].parent;
            }
            actionList->addAction(new MarkPathAction(currentNode.x, currentNode.y));


        }
    };

    class BreadthFirstCalculator {
    public:
        static void calculate(Grid* grid, ActionList* actionList) {

            actionList->addAction(new NoneAction());

            Node** nodes;
            std::queue<sf::Vector2i> nodeQueue;

            int numRows = grid->getNumRows();
            int numCols = grid->getNumCols();
            sf::Vector2i startNode = grid->getStartNode();
            sf::Vector2i endNode = grid->getEndNode();
            sf::Vector2i currentNode = startNode;
            sf::Vector2i rightNode, leftNode, topNode, bottomNode;

            nodes = new Node* [numRows];
            for (int i = 0; i < numRows; i++)
            {
                nodes[i] = new Node[numCols];
                
            }

            nodeQueue.push(startNode);
            nodes[startNode.x][startNode.y].visited = true;
            actionList->addAction(new MarkVisitedAction(startNode.x, startNode.y));

            while (currentNode != endNode && !nodeQueue.empty())
            {
                currentNode = nodeQueue.front();
                nodeQueue.pop();

                rightNode.x = currentNode.x + 1;
                rightNode.y = currentNode.y;
                leftNode.x = currentNode.x - 1;
                leftNode.y = currentNode.y;
                topNode.x = currentNode.x;
                topNode.y = currentNode.y - 1;
                bottomNode.x = currentNode.x;
                bottomNode.y = currentNode.y + 1;


                if (grid->isValidPosition(rightNode) && !nodes[rightNode.x][rightNode.y].visited && !grid->isWall(rightNode.x, rightNode.y))
                {
                    nodeQueue.emplace(rightNode);
                    nodes[rightNode.x][rightNode.y].visited = true;
                    nodes[rightNode.x][rightNode.y].parent = currentNode;
                    actionList->addAction(new MarkVisitedAction(rightNode.x, rightNode.y));
                    if (rightNode == endNode) {
                        currentNode = endNode;
                        break;
                    }
                }
                if (grid->isValidPosition(leftNode) && !nodes[leftNode.x][leftNode.y].visited && !grid->isWall(leftNode.x, leftNode.y))
                {
                    nodeQueue.emplace(leftNode);
                    nodes[leftNode.x][leftNode.y].visited = true;
                    nodes[leftNode.x][leftNode.y].parent = currentNode;
                    actionList->addAction(new MarkVisitedAction(leftNode.x, leftNode.y));
                    if (leftNode == endNode) {
                        currentNode = endNode;
                        break;
                    }
                }
                if (grid->isValidPosition(topNode) && !nodes[topNode.x][topNode.y].visited && !grid->isWall(topNode.x, topNode.y))
                {
                    nodeQueue.emplace(topNode);
                    nodes[topNode.x][topNode.y].visited = true;
                    nodes[topNode.x][topNode.y].parent = currentNode;
                    actionList->addAction(new MarkVisitedAction(topNode.x, topNode.y));
                    if (topNode == endNode) {
                        currentNode = endNode;
                        break;
                    }
                }
                if (grid->isValidPosition(bottomNode) && !nodes[bottomNode.x][bottomNode.y].visited && !grid->isWall(bottomNode.x, bottomNode.y))
                {
                    nodeQueue.emplace(bottomNode);
                    nodes[bottomNode.x][bottomNode.y].visited = true;
                    nodes[bottomNode.x][bottomNode.y].parent = currentNode;
                    actionList->addAction(new MarkVisitedAction(bottomNode.x, bottomNode.y));
                    if (bottomNode == endNode) {
                        currentNode = endNode;
                        break;
                    }
                }
            }

            if (currentNode == endNode)
            {
                reconstructPath(nodes, grid, actionList);
            }

            for (int i = 0; i < numRows; i++) {
                delete[] nodes[i];
            }
            delete[] nodes;


            actionList->addAction(new NoneAction());
        }
    private:
        struct Node {
        public:
            bool visited = false;
            sf::Vector2i parent;
            Node() {
            }
        };

        static void reconstructPath(Node** nodes, Grid* grid, ActionList* actionList) {

            sf::Vector2i startNode = grid->getStartNode();
            sf::Vector2i endNode = grid->getEndNode();

            sf::Vector2i currentNode = endNode;

            while (currentNode != startNode)
            {
                actionList->addAction(new MarkPathAction(currentNode.x, currentNode.y));
                currentNode = nodes[currentNode.x][currentNode.y].parent;
            }
            actionList->addAction(new MarkPathAction(currentNode.x, currentNode.y));


        }
    };

    class BidirectionalCalculator {
    public:
        static void calculate(Grid* grid, ActionList* actionList) {

            actionList->addAction(new NoneAction());

            Node** nodes;
            std::queue<sf::Vector2i> nodeQueue1;
            std::queue<sf::Vector2i> nodeQueue2;

            int numRows = grid->getNumRows();
            int numCols = grid->getNumCols();
            sf::Vector2i startNode = grid->getStartNode();
            sf::Vector2i endNode = grid->getEndNode();
            sf::Vector2i currentNode1 = startNode;
            sf::Vector2i currentNode2 = endNode;
            sf::Vector2i rightNode1, leftNode1, topNode1, bottomNode1;
            sf::Vector2i rightNode2, leftNode2, topNode2, bottomNode2;
            sf::Vector2i impactNode1, impactNode2;

            nodes = new Node * [numRows];
            for (int i = 0; i < numRows; i++)
            {
                nodes[i] = new Node[numCols];

            }

            nodeQueue1.push(startNode);
            nodes[startNode.x][startNode.y].visited1 = true;
            actionList->addAction(new MarkVisitedAction(startNode.x, startNode.y));

            nodeQueue2.push(endNode);
            nodes[endNode.x][endNode.y].visited2 = true;
            actionList->addAction(new MarkVisitedAction(endNode.x, endNode.y));

            bool found = false;

            while (!found && !nodeQueue1.empty() && !nodeQueue2.empty())
            {
                currentNode1 = nodeQueue1.front();
                nodeQueue1.pop();

                currentNode2 = nodeQueue2.front();
                nodeQueue2.pop();

                rightNode1.x = currentNode1.x + 1;
                rightNode1.y = currentNode1.y;
                leftNode1.x = currentNode1.x - 1;
                leftNode1.y = currentNode1.y;
                topNode1.x = currentNode1.x;
                topNode1.y = currentNode1.y - 1;
                bottomNode1.x = currentNode1.x;
                bottomNode1.y = currentNode1.y + 1;

                rightNode2.x = currentNode2.x + 1;
                rightNode2.y = currentNode2.y;
                leftNode2.x = currentNode2.x - 1;
                leftNode2.y = currentNode2.y;
                topNode2.x = currentNode2.x;
                topNode2.y = currentNode2.y - 1;
                bottomNode2.x = currentNode2.x;
                bottomNode2.y = currentNode2.y + 1;

                //first front

                if (grid->isValidPosition(rightNode1) && !nodes[rightNode1.x][rightNode1.y].visited1 && !grid->isWall(rightNode1.x, rightNode1.y))
                {
                    nodeQueue1.emplace(rightNode1);
                    nodes[rightNode1.x][rightNode1.y].visited1 = true;
                    impactNode2 = nodes[rightNode1.x][rightNode1.y].parent;
                    nodes[rightNode1.x][rightNode1.y].parent = currentNode1;
                    
                    if (rightNode1 == endNode || nodes[rightNode1.x][rightNode1.y].visited2) {
                        found = true;
                        impactNode1 = rightNode1;
                        break;
                    }
                    actionList->addAction(new MarkVisitedAction(rightNode1.x, rightNode1.y));
                }
                if (grid->isValidPosition(leftNode1) && !nodes[leftNode1.x][leftNode1.y].visited1 && !grid->isWall(leftNode1.x, leftNode1.y))
                {
                    nodeQueue1.emplace(leftNode1);
                    nodes[leftNode1.x][leftNode1.y].visited1 = true;
                    impactNode2 = nodes[leftNode1.x][leftNode1.y].parent;
                    nodes[leftNode1.x][leftNode1.y].parent = currentNode1;
                    
                    if (leftNode1 == endNode || nodes[leftNode1.x][leftNode1.y].visited2) {
                        found = true;
                        impactNode1 = leftNode1;
                        break;
                    }
                    actionList->addAction(new MarkVisitedAction(leftNode1.x, leftNode1.y));
                }
                if (grid->isValidPosition(topNode1) && !nodes[topNode1.x][topNode1.y].visited1 && !grid->isWall(topNode1.x, topNode1.y))
                {
                    nodeQueue1.emplace(topNode1);
                    nodes[topNode1.x][topNode1.y].visited1 = true;
                    impactNode2 = nodes[topNode1.x][topNode1.y].parent;
                    nodes[topNode1.x][topNode1.y].parent = currentNode1;
                    
                    if (topNode1 == endNode || nodes[topNode1.x][topNode1.y].visited2) {
                        found = true;
                        impactNode1 = topNode1;
                        break;
                    }
                    actionList->addAction(new MarkVisitedAction(topNode1.x, topNode1.y));
                }
                if (grid->isValidPosition(bottomNode1) && !nodes[bottomNode1.x][bottomNode1.y].visited1 && !grid->isWall(bottomNode1.x, bottomNode1.y))
                {
                    nodeQueue1.emplace(bottomNode1);
                    nodes[bottomNode1.x][bottomNode1.y].visited1 = true;
                    impactNode2 = nodes[bottomNode1.x][bottomNode1.y].parent;
                    nodes[bottomNode1.x][bottomNode1.y].parent = currentNode1;
                    
                    if (bottomNode1 == endNode || nodes[bottomNode1.x][bottomNode1.y].visited2) {
                        found = true;
                        impactNode1 = bottomNode1;
                        break;
                    }
                    actionList->addAction(new MarkVisitedAction(bottomNode1.x, bottomNode1.y));
                }

                //second front

                if (grid->isValidPosition(rightNode2) && !nodes[rightNode2.x][rightNode2.y].visited2 && !grid->isWall(rightNode2.x, rightNode2.y))
                {
                    nodeQueue2.emplace(rightNode2);
                    nodes[rightNode2.x][rightNode2.y].visited2 = true;
                    impactNode1 = nodes[rightNode2.x][rightNode2.y].parent;
                    nodes[rightNode2.x][rightNode2.y].parent = currentNode2;
                    
                    if (rightNode2 == startNode || nodes[rightNode2.x][rightNode2.y].visited1) {
                        found = true;
                        impactNode2 = rightNode2;
                        break;
                    }
                    actionList->addAction(new MarkVisitedAction(rightNode2.x, rightNode2.y));
                }
                if (grid->isValidPosition(leftNode2) && !nodes[leftNode2.x][leftNode2.y].visited2 && !grid->isWall(leftNode2.x, leftNode2.y))
                {
                    nodeQueue2.emplace(leftNode2);
                    nodes[leftNode2.x][leftNode2.y].visited2 = true;
                    impactNode1 = nodes[leftNode2.x][leftNode2.y].parent;
                    nodes[leftNode2.x][leftNode2.y].parent = currentNode2;
                    
                    if (leftNode2 == startNode || nodes[leftNode2.x][leftNode2.y].visited1) {
                        found = true;
                        impactNode2 = leftNode2;
                        break;
                    }
                    actionList->addAction(new MarkVisitedAction(leftNode2.x, leftNode2.y));
                }
                if (grid->isValidPosition(topNode2) && !nodes[topNode2.x][topNode2.y].visited2 && !grid->isWall(topNode2.x, topNode2.y))
                {
                    nodeQueue2.emplace(topNode2);
                    nodes[topNode2.x][topNode2.y].visited2 = true;
                    impactNode1 = nodes[topNode2.x][topNode2.y].parent;
                    nodes[topNode2.x][topNode2.y].parent = currentNode2;
                    actionList->addAction(new MarkVisitedAction(topNode2.x, topNode2.y));
                    if (topNode2 == startNode || nodes[topNode2.x][topNode2.y].visited1) {
                        found = true;
                        impactNode2 = topNode2;
                        break;
                    }
                }
                if (grid->isValidPosition(bottomNode2) && !nodes[bottomNode2.x][bottomNode2.y].visited2 && !grid->isWall(bottomNode2.x, bottomNode2.y))
                {
                    nodeQueue2.emplace(bottomNode2);
                    nodes[bottomNode2.x][bottomNode2.y].visited2 = true;
                    impactNode1 = nodes[bottomNode2.x][bottomNode2.y].parent;
                    nodes[bottomNode2.x][bottomNode2.y].parent = currentNode2;
                    
                    if (bottomNode2 == startNode || nodes[bottomNode2.x][bottomNode2.y].visited1) {
                        found = true;
                        impactNode2 = bottomNode2;
                        break;
                    }

                    actionList->addAction(new MarkVisitedAction(bottomNode2.x, bottomNode2.y));
                }

            }

            if (found)
            {
                reconstructPath(impactNode1, impactNode2, nodes, grid, actionList);
            }

            for (int i = 0; i < numRows; i++) {
                delete[] nodes[i];
            }
            delete[] nodes;


            actionList->addAction(new NoneAction());
        }
    private:
        struct Node {
        public:
            bool visited1 = false;
            bool visited2 = false;
            sf::Vector2i parent = sf::Vector2i(-1, -1);
            Node() {
            }
        };

        static void reconstructPath(sf::Vector2i impactNode1, sf::Vector2i impactNode2, Node** nodes, Grid* grid, ActionList* actionList) {

            sf::Vector2i startNode = grid->getStartNode();
            sf::Vector2i endNode = grid->getEndNode();

            while (impactNode1 != startNode && impactNode1.x != -1)
            {
                actionList->addAction(new MarkPathAction(impactNode1.x, impactNode1.y));
                impactNode1 = nodes[impactNode1.x][impactNode1.y].parent;
            }
            
            if (impactNode1.x != -1)
            {
                actionList->addAction(new MarkPathAction(impactNode1.x, impactNode1.y));
            }
           

            while (impactNode2 != endNode && impactNode2.x != -1)
            {
                actionList->addAction(new MarkPathAction(impactNode2.x, impactNode2.y));
                impactNode2 = nodes[impactNode2.x][impactNode2.y].parent;
            }
            if (impactNode2.x != -1)
            {
                actionList->addAction(new MarkPathAction(impactNode2.x, impactNode2.y));
            }

        }
    };




public:


	static void PathFind(Grid* grid, ActionList* actionList, PathFindType pathFindType) {
		switch (pathFindType)
		{
        case PathFindType::Dijkstra:DijkstraCalculator::calculate(grid, actionList);
			break;
        case PathFindType::AStar:AStarCalculator::calculate(grid, actionList);
			break;
        case PathFindType::DepthFirst:DepthFirstCalculator::calculate(grid, actionList);
            break;
        case PathFindType::BreadthFirst:BreadthFirstCalculator::calculate(grid, actionList);
            break;
        case PathFindType::Bidirectional:BidirectionalCalculator::calculate(grid, actionList);
            break;
		default:
			break;
		}
	}

};

