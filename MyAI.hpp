// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Jian Li
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#ifndef MINE_SWEEPER_CPP_SHELL_MYAI_HPP
#define MINE_SWEEPER_CPP_SHELL_MYAI_HPP

#include "Agent.hpp"
#include <iostream> // temporary use
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <unordered_set>
#include <queue>
#include <cmath>

using namespace std;

class MyAI : public Agent
{
public:
    MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY );

    Action getAction ( int number ) override;

    
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================



    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================


private:

    struct Mines {
        int parent;
        int row;
        int col;

        bool operator<(const Mines& m1) const
        {
            return parent > m1.parent;
        }
    };

    
    struct UncoveredFrontier{
        int row;
        int col;
        int effLabel;
    };


    struct CoveredUnmarkedFrontier{
        int row;
        int col;
        int value = -2;
        int sc = 0;
        double percent;
        set<pair<int, int>> uncoveredFrontier;
    };


    int rowDimension;
    int colDimension;
    int totalMines;

    int visitingRow;
    int visitingCol;

    int numCovered;

    int solutionCount;

    bool visitedStart;
    bool poppingRest;

    int** board;

    void initializeBoard();

    void printBoard();

    int NumUnMarkedNeighbors(int row, int col);

    bool validPos(int row, int col);

    void QueueForVisiting(int row, int col);

    void remaining();

    void popClosest();

    void ROTenum();

    void FlagSurroundingTile(int row, int col);

    void ModelChecking();

    bool CheckVarAssignment(vector<UncoveredFrontier> c, vector<CoveredUnmarkedFrontier> v);

    vector<pair<int, int>> findUncoveredWithMines(int row, int col);

    vector<pair<int, int>> findUncovered(int row, int col);

    vector<pair<int, int>> findCovered(int row, int col);

    vector<CoveredUnmarkedFrontier> findCUF();

    vector<UncoveredFrontier> findUF();

    queue<Mines> ROTvisiting;
    
    void backTracking(int index, vector<CoveredUnmarkedFrontier> v, vector<UncoveredFrontier> c, vector<CoveredUnmarkedFrontier> &solution);

    set<pair<int, int>> visited;


};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP
