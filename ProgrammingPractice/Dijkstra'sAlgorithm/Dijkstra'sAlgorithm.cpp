#include <string>
#include <iostream>
#include <vector>
#include <math.h>   
#include <algorithm>

using namespace std;

struct node
{
    pair<int, int> position;
    double distanceFromStartNode = 0;
    pair<int, int> previousPosition;
};

int findSmallestNumbersOfCarsToMove(char** map, int numberOfRows, int numberOfColumns, int startRow, int startColumn);
vector<pair<int, int>> findShortestPathBetweenTwoPoints(char** map, int numberOfRows, int numberOfColumns, pair<int, int> startingPosition, pair<int, int> targetPosition);
vector<pair<int, int>> getNeighboursOfANode(char** map, pair<int, int> currentPosition, int numberOfRows, int numberOfColumns, vector<node> visitedNodes);
int returnPositionOfPairOfIntsInAVector(pair<int, int> elementToSearchFor, vector<node> listToSearchIn);
bool sortByDistanceFromStartNode(node& a, node& b);

int findSmallestNumbersOfCarsToMove(char** map, int numberOfRows, int numberOfColumns, int startRow, int startColumn)
{
    //find exit doors
    vector<pair<int, int>> exitDoors = vector<pair<int, int>>();

    for (int i = 0; i < numberOfRows; i++)
    {
        if (i == 0 || i == numberOfRows - 1)
        {
            for (int j = 0; j < numberOfColumns; j++)
            {
                if (map[i][j] == 'D')
                {
                    exitDoors.push_back(pair<int, int>(i + 1, j + 1));
                }
            }
        }
        else
        {
            for (int j = 0; j < 2; j++)
            {
                int columnNumber = j == 0 ? 0 : numberOfColumns - 1;
                
                if (map[i][columnNumber] == 'D')
                {
                    exitDoors.push_back(pair<int, int>(i + 1, columnNumber + 1));
                }
            }
        }
    }

    //from starting point, for all exit doors
    //find path to exit using Dijkstra's algorithm
    //empty list if no path exists
    vector<vector<pair<int, int>>> paths = vector<vector<pair<int, int>>>();

    for (size_t i = 0; i < exitDoors.size(); i++)
    {
        paths.push_back(findShortestPathBetweenTwoPoints(map, numberOfRows, numberOfColumns, pair<int, int>(startRow, startColumn), exitDoors[i]));
    }

    //count number of cars that need to be moved in all paths
    int numberOfCarsToMove = 0;

    for (size_t i = 0; i < paths.size(); i++)
    {
        int temp = 1;

        for (size_t j = 0; j < paths[i].size(); j++)
        {
            pair<int, int> currentPosition = paths[i][j];

            if (map[currentPosition.first - 1][currentPosition.second - 1] == 'c')
            {
                temp++;
            }
        }

        if (paths[i].size() > 0 && (numberOfCarsToMove == 0 || temp < numberOfCarsToMove))
        {
            numberOfCarsToMove = temp;
        }
    }

    return numberOfCarsToMove;
}

vector<pair<int, int>> findShortestPathBetweenTwoPoints(char** map, int numberOfRows, int numberOfColumns, pair<int, int> startingPosition, pair<int, int> targetPosition)
{
    vector<node> unvisitedNodes = vector<node>();
    vector<node> visitedNodes = vector<node>();

    bool targetReached = false;

    node startNode;
    startNode.position = startingPosition;
    unvisitedNodes.push_back(startNode);

    int distanceFromStartNode = 0;
    
    node currentNode = unvisitedNodes[0];

    while (!targetReached)
    {
        if (unvisitedNodes.size() == 0 && currentNode.position != startingPosition)
        {
            return vector<pair<int, int>>();
        }
        
        currentNode = *std::min_element(unvisitedNodes.begin(), unvisitedNodes.end(), sortByDistanceFromStartNode);
        unvisitedNodes.erase(unvisitedNodes.begin());
        visitedNodes.push_back(currentNode);

        if (currentNode.position == targetPosition)
        {
            break;
        }

        vector<pair<int, int>> neighbours = getNeighboursOfANode(map, currentNode.position, numberOfRows, numberOfColumns, visitedNodes);

        int updatedDistanceFromStartNode = distanceFromStartNode + 1;

        for (size_t i = 0; i < neighbours.size(); i++)
        {
            //check if vertex is in unvisitedNodes list
            int indexOfVertex = returnPositionOfPairOfIntsInAVector(neighbours[i], unvisitedNodes);

            if (indexOfVertex > -1)
            {
                //if the calculated distance of this vertex is less than the known distance
                //update shortest distance to this vertex
                //update the previous vertex with the current vertex

                if (updatedDistanceFromStartNode < unvisitedNodes[indexOfVertex].distanceFromStartNode)
                {
                    unvisitedNodes[indexOfVertex].distanceFromStartNode = updatedDistanceFromStartNode;
                    unvisitedNodes[indexOfVertex].previousPosition = currentNode.position;
                }
            }
            else
            {
                //else add vertex to list, only add if not visited
                if (returnPositionOfPairOfIntsInAVector(neighbours[i], visitedNodes) == -1)
                {
                    node newVertex;
                    newVertex.distanceFromStartNode = updatedDistanceFromStartNode;
                    newVertex.position = neighbours[i];
                    newVertex.previousPosition = currentNode.position;

                    unvisitedNodes.push_back(newVertex);
                }
            }                        
        }

        distanceFromStartNode++;
    }

    //create list of nodes to target node
    vector<pair<int, int>> path = vector<pair<int, int>>();

    bool startNodeReached = false;
    node currentPosition = visitedNodes[visitedNodes.size() - 1];

    while (startNodeReached == false)
    {
        if (currentPosition.position == startNode.position)
        {
            break;
        }

        path.push_back(currentPosition.position);

        for (size_t i = 0; i < visitedNodes.size(); i++)
        {
            if (visitedNodes[i].position == currentPosition.previousPosition)
            {
                currentPosition = visitedNodes[i];
                break;
            }
        }
    }

    reverse(path.begin(), path.end());

    return path;
}

vector<pair<int, int>> getNeighboursOfANode(char** map, pair<int, int> currentPosition, int numberOfRows, int numberOfColumns, vector<node> visitedNodes)
{
    vector<pair<int, int>> neighbouringNodes = vector<pair<int, int>>();

    if (currentPosition.first - 1 > 0 && map[currentPosition.first - 2][currentPosition.second - 1] != '#')
    {
        neighbouringNodes.push_back(pair<int, int>(currentPosition.first - 1, currentPosition.second));
    }

    if (currentPosition.first + 1 <= numberOfRows && map[currentPosition.first][currentPosition.second - 1] != '#')
    {
        neighbouringNodes.push_back(pair<int, int>(currentPosition.first + 1, currentPosition.second));
    }

    if (currentPosition.second - 1 > 0 && map[currentPosition.first - 1][currentPosition.second - 2] != '#')
    {
        neighbouringNodes.push_back(pair<int, int>(currentPosition.first, currentPosition.second - 1));
    }

    if (currentPosition.second + 1 <= numberOfColumns && map[currentPosition.first - 1][currentPosition.second] != '#')
    {
        neighbouringNodes.push_back(pair<int, int>(currentPosition.first, currentPosition.second + 1));
    }

    return neighbouringNodes;
}

int returnPositionOfPairOfIntsInAVector(pair<int, int> elementToSearchFor, vector<node> listToSearchIn)
{
    for (size_t i = 0; i < listToSearchIn.size(); i++)
    {
        if (listToSearchIn[i].position == elementToSearchFor)
        {
            return i;
        }
    }

    return -1;
}

bool sortByDistanceFromStartNode(node& a, node& b)
{
    return a.distanceFromStartNode < b.distanceFromStartNode;
}

int main(int argc, char* argv[])
{
    int numberOfRows = 0;
    int numberOfColumns = 0;

    scanf_s("%d %d", &numberOfRows, &numberOfColumns);

    char** map = new char* [numberOfRows];
    for (int i = 0; i < numberOfRows; i++)
    {
        map[i] = new char[numberOfColumns];
    }

    for (int i = 0; i < numberOfRows; i++)
    {
        for (int j = 0; j < numberOfColumns; j++)
        {
            scanf_s(" %c", &map[i][j], 1);
        }
    }

    int startRow = 0;
    int startColumn = 0;

    scanf_s(" %d %d", &startRow, &startColumn);

    printf("%d\n", findSmallestNumbersOfCarsToMove(map, numberOfRows, numberOfColumns, startRow, startColumn));

    for (int i = 0; i < numberOfRows; i++)
    {
        delete map[i];
    }

    delete[] map;
}