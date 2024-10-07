// ======================================================================
// FILE:        MyAI.cpp
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

#include "MyAI.hpp"

MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) : Agent()
{
    rowDimension = _rowDimension;
    colDimension = _colDimension;
    totalMines = _totalMines;
    visitingRow = _agentY;
    visitingCol = _agentX;
    visitedStart = false;
    numCovered = 0;
    poppingRest = false;
    initializeBoard();
};

void MyAI::printBoard()
{
    for (int i = 0; i < rowDimension; i++) {
        for (int j = 0; j < colDimension; j++) {
 
            // Print the values of
            // memory blocks created
            if (board[i][j] != -2 && board[i][j] != -1)
            {
                cout << " " << board[i][j] << " ";
                continue;
            }
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\n" << endl; 
}

Agent::Action MyAI::getAction( int number )
{
    //cout << "row: " << visitingRow << " col: " << visitingCol << endl;
    if (visitedStart == true)
    {
        board[visitingRow][visitingCol] = number;
        vector<pair<int, int>> temp = findUncoveredWithMines(visitingRow, visitingCol);
        for (int i = 0; i < temp.size(); ++i)
        {
            if (board[temp[i].first][temp[i].second] == -1)
            {
                board[visitingRow][visitingCol]--;
                if (board[visitingRow][visitingCol] == 0)
                {
                    QueueForVisiting(visitingRow, visitingCol);
                }
            }
        }
    }

    //cout << "row: " << visitingRow << " col: " << visitingCol << endl;
    //printBoard();
    if (visitedStart == false)
    {
        board[visitingRow][visitingCol] == 0;
        visitedStart = true;
        numCovered++;
        return {UNCOVER, visitingCol, visitingRow};
    }

    if (number == 0)
    {
        QueueForVisiting(visitingRow, visitingCol);
    }

    if (ROTvisiting.size() > 0)
    {
        Mines m1 = ROTvisiting.front();
        ROTvisiting.pop();
        numCovered++;
        visitingRow = m1.row;
        visitingCol = m1.col;
        //cout << "problem row:" << visitingRow << " problem col: " << visitingCol << endl;
        return{UNCOVER, m1.col, m1.row};
    }
    ROTenum();

    if (ROTvisiting.size() > 0)
    {
        Mines m1 = ROTvisiting.front();
        ROTvisiting.pop();
        numCovered++;
        visitingRow = m1.row;
        visitingCol = m1.col;
        return{UNCOVER, m1.col, m1.row};
    }

    ModelChecking();

    //printBoard();
    //cout << "Queue for visiting size: " << ROTvisiting.size() << endl;

    if (ROTvisiting.size() > 0)
    {
        Mines m1 = ROTvisiting.front();
        ROTvisiting.pop();
        numCovered++;
        visitingRow = m1.row;
        visitingCol = m1.col;
        //cout << "problem row:" << visitingRow << " problem col: " << visitingCol << endl;
        return{UNCOVER, m1.col, m1.row};
    }

    
    return {LEAVE,-1,-1};
}


void MyAI::ModelChecking()
{
    solutionCount = 0;
    vector<CoveredUnmarkedFrontier> v;
    vector<UncoveredFrontier> c;
    v = findCUF();
    //cout << "no problem here at CUF" << endl;
    c = findUF();
    //cout << "no problem here at UF" << endl;
    /*
    for (int i = 0; i < v.size(); ++i)
    {
        cout << "v row: " << v[i].row << " col: " << v[i].col << endl;
    }
    vector<pair<int, int>> abc;
    abc = findUncovered(6, 4);
    cout << " 6 4: " << board[6][4] << " " << abc.size() << endl;
    cout << "v size: " << v.size() << endl;
    cout << "c size: " << c.size() << endl;
    */

    if (v.size() == 0)
    {
        return;
    }
    backTracking(0, v, c, v);

    /*
    cout << "solution count: " << solutionCount << endl;
    for (int i = 0; i < v.size(); ++i)
    {
        cout << "v row: " << v[i].row << " col: " << v[i].col << " value: " << v[i].sc << endl;
    }
    */
   int count = 0;
   for (int i = 0; i < v.size(); ++i)
   {
       v[i].percent = v[i].sc/(double)solutionCount;
       if (v[i].percent == 0)
       {
           count++;
           //cout << "ModelChecking push row: " << v[i].row << " col: " << v[i].col << endl;
           ROTvisiting.push({0, v[i].row, v[i].col});
       }
       else if (v[i].percent == 1)
       {
           count++;
           board[v[i].row][v[i].col] = -1;
           vector<pair<int, int>> temp = findUncovered(v[i].row, v[i].col);
           for (int j = 0; j < temp.size(); ++j)
           {
               board[temp[j].first][temp[j].second]--;
               if (board[temp[j].first][temp[j].second] == 0)
               {
                   QueueForVisiting(temp[j].first, temp[j].second);
               }
           }
       }
   }

   if (count == 0)
   {
       double smallest = 2.0;
       int row;
       int col;
       for (int i = 0; i < v.size(); ++i)
       {
           if (v[i].percent < smallest)
           {
               smallest = v[i].percent;
               row = v[i].row;
               col = v[i].col;
           }
       }
       ROTvisiting.push({0, row, col});
   }

}

void MyAI::backTracking(int index, vector<CoveredUnmarkedFrontier> v, vector<UncoveredFrontier> c, vector<CoveredUnmarkedFrontier> &solution)
{
    v[index].value = 0;
    //cout << "index: " << index << " value: " << 0 << endl;
    if (CheckVarAssignment(c, v) == true)
    {
        if (index == v.size() - 1)
        {
            solutionCount++;
            for (int i = 0; i < v.size(); ++i)
            {
                if (v[i].value == 1)
                {
                    solution[i].sc++;
                }
            }
            return;
        }
        backTracking(index+1, v, c, solution);
    }
    v[index].value = 1;
    //cout << "index: " << index << " value: " << 1 << endl;
    if (CheckVarAssignment(c, v) == true)
    {
        if (index == v.size() - 1)
        {
            solutionCount++;
            for (int i = 0; i < v.size(); ++i)
            {
                if (v[i].value == 1)
                {
                    solution[i].sc++;
                }
            }
            return;
        }
        backTracking(index+1, v, c, solution);
    }
}

bool MyAI::CheckVarAssignment(vector<UncoveredFrontier> c, vector<CoveredUnmarkedFrontier> v)
{
    for (int i = 0; i < c.size(); ++i)
    {
        int unassigned = 0;
        int mines = 0;
        for (int j = 0; j < v.size(); ++j)
        {
            if (v[j].uncoveredFrontier.find({c[i].row, c[i].col}) != v[j].uncoveredFrontier.end())
            {
                if (v[j].value == -2)
                {
                    unassigned++;
                }
                else if (v[j].value == 1)
                {
                    mines++;
                }
                else if (v[j].value == 0)
                {
                    continue;
                }
            }
        }
        if (mines > c[i].effLabel || c[i].effLabel > (mines+unassigned))
        {
            return false;
        }
    }
    return true;
}


vector<MyAI::UncoveredFrontier> MyAI::findUF()
{
    vector<UncoveredFrontier> temp;
    for (int i = 0; i < rowDimension; ++i)
    {
        for (int j = 0; j < colDimension; ++j)
        {
            if (board[i][j] == -1 || board[i][j] == -2)
            {
                continue;
            }
            vector<pair<int, int>> temp1 = findCovered(i, j);
            if (temp1.size() > 0)
            {
                temp.push_back({i, j, board[i][j]});
            }
        }
    }
    return temp;
}



vector<MyAI::CoveredUnmarkedFrontier> MyAI::findCUF()
{
    vector<CoveredUnmarkedFrontier> temp;
    for (int i = 0; i < rowDimension; ++i)
    {
        for (int j = 0; j < colDimension; ++j)
        {
            if (board[i][j] == -1)
            {
                continue;
            }

            if (board[i][j] == -2)
            {
                vector<pair<int, int>> temp1 = findUncovered(i, j);
                if (temp1.size() > 0)
                {
                    CoveredUnmarkedFrontier x;
                    x.row = i;
                    x.col = j;
                    for (int k = 0; k < temp1.size(); ++k)
                    {
                        UncoveredFrontier y;
                        x.uncoveredFrontier.insert({temp1[k].first, temp1[k].second});
                    }
                    temp.push_back(x);
                }
            }
        }
    }
    return temp;
}








void MyAI::ROTenum()
{
    for (int i = 0; i < rowDimension; ++i)
    {
        for (int j = 0; j < colDimension; ++j)
        {
            if (board[i][j] != 0 && board[i][j] != -2 && board[i][j] != -1)
            {
                if (NumUnMarkedNeighbors(i, j) == board[i][j])
                {
                    FlagSurroundingTile(i, j);
                }
            }
        }
    }

    for (int i = 0; i < rowDimension; ++i)
    {
        for (int j = 0; j < colDimension; ++j)
        {
            if (board[i][j] == 0 && NumUnMarkedNeighbors(i, j) != 0)
            {
                QueueForVisiting(i, j);
            }
        }
    
    }

}

void MyAI::FlagSurroundingTile(int row, int col)
{
    vector<pair<int, int>> temp = findCovered(row, col);
    vector<pair<int, int>> temp2;

    for (int i = 0; i < temp.size(); ++i)
    {
        board[temp[i].first][temp[i].second] = -1;
        temp2 = findUncovered(temp[i].first, temp[i].second);
        for (int j = 0; j < temp2.size(); ++j)
        {
            if (board[temp2[j].first][temp2[j].second] != -1)
            {
                board[temp2[j].first][temp2[j].second]--;
            }
        }
    }

}


vector<pair<int, int>> MyAI::findCovered(int row, int col)
{
    vector<pair<int, int>> temp;

    if (validPos(row, col+1) && board[row][col+1] == -2)
    {
        temp.push_back({row, col+1});
    }
    if (validPos(row, col-1) && board[row][col-1] == -2)
    {
        temp.push_back({row, col-1});
    }
    if (validPos(row+1, col) && board[row+1][col] == -2)
    {
        temp.push_back({row+1, col});
    }
    if (validPos(row-1, col) && board[row-1][col] == -2)
    {
        temp.push_back({row-1, col});
    }
    if (validPos(row-1, col+1) && board[row-1][col+1] == -2)
    {
        temp.push_back({row-1, col+1});
    }
    if (validPos(row-1, col-1) && board[row-1][col-1] == -2)
    {
        temp.push_back({row-1, col-1});
    }
    if (validPos(row+1, col+1) && board[row+1][col+1] == -2)
    {
        temp.push_back({row+1, col+1});
    }
    if (validPos(row+1, col-1) && board[row+1][col-1] == -2)
    {
        temp.push_back({row+1, col-1});
    }
    return temp;
}

vector<pair<int, int>> MyAI::findUncoveredWithMines(int row, int col)
{
    vector<pair<int, int>> temp;

    if (validPos(row, col+1) && board[row][col+1] != -2)
    {
        temp.push_back({row, col+1});
    }
    if (validPos(row, col-1) && board[row][col-1] != -2)
    {
        temp.push_back({row, col-1});
    }
    if (validPos(row+1, col) && board[row+1][col] != -2)
    {
        temp.push_back({row+1, col});
    }
    if (validPos(row-1, col) && board[row-1][col] != -2)
    {
        temp.push_back({row-1, col});
    }
    if (validPos(row-1, col+1) && board[row-1][col+1] != -2)
    {
        temp.push_back({row-1, col+1});
    }
    if (validPos(row-1, col-1) && board[row-1][col-1] != -2)
    {
        temp.push_back({row-1, col-1});
    }
    if (validPos(row+1, col+1) && board[row+1][col+1] != -2)
    {
        temp.push_back({row+1, col+1});
    }
    if (validPos(row+1, col-1) && board[row+1][col-1] != -2)
    {
        temp.push_back({row+1, col-1});
    }
    return temp;
}


vector<pair<int, int>> MyAI::findUncovered(int row, int col)
{
    vector<pair<int, int>> temp;

    if (validPos(row, col+1) && board[row][col+1] != -2 && board[row][col+1] != -1)
    {
        temp.push_back({row, col+1});
    }
    if (validPos(row, col-1) && board[row][col-1] != -2 && board[row][col-1] != -1)
    {
        temp.push_back({row, col-1});
    }
    if (validPos(row+1, col) && board[row+1][col] != -2 && board[row+1][col] != -1)
    {
        temp.push_back({row+1, col});
    }
    if (validPos(row-1, col) && board[row-1][col] != -2 && board[row-1][col] != -1)
    {
        temp.push_back({row-1, col});
    }
    if (validPos(row-1, col+1) && board[row-1][col+1] != -2 && board[row-1][col+1] != -1)
    {
        temp.push_back({row-1, col+1});
    }
    if (validPos(row-1, col-1) && board[row-1][col-1] != -2 && board[row-1][col-1] != -1)
    {
        temp.push_back({row-1, col-1});
    }
    if (validPos(row+1, col+1) && board[row+1][col+1] != -2 && board[row+1][col+1] != -1)
    {
        temp.push_back({row+1, col+1});
    }
    if (validPos(row+1, col-1) && board[row+1][col-1] != -2 && board[row+1][col-1] != -1)
    {
        temp.push_back({row+1, col-1});
    }
    return temp;
}


void MyAI::initializeBoard()
{
    board = new int*[rowDimension];
    for (int i = 0; i < rowDimension; ++i)
    {
        board[i] = new int [colDimension];
    }

    for (int i = 0; i < rowDimension; ++i)
    {
        for (int j = 0; j < colDimension; ++j)
        {
            board[i][j] = -2;
        }
    }
}


void MyAI::QueueForVisiting(int row, int col)
{
    if (NumUnMarkedNeighbors(row, col) == 0)
    {
        return;
    }

    if (validPos(row, col+1) && board[row][col+1] == -2 && visited.find({row, col+1}) == visited.end())
    {
        //cout << "QueueForVisiting row: " << row << " QueueForVisiting Col: " << col << endl;
        //cout << "Queue Pushing row: " << row << " Queue Pushing Col: " << col+1 << endl; 
        ROTvisiting.push(Mines{0, row, col+1});
        visited.insert({row, col+1});
    }
    if (validPos(row, col-1) && board[row][col-1] == -2 && visited.find({row, col-1}) == visited.end())
    {
        //cout << "QueueForVisiting row: " << row << " QueueForVisiting Col: " << col << endl;
        //cout << "Queue Pushing row: " << row << " Queue Pushing Col: " << col-1 << endl;
        ROTvisiting.push(Mines{0, row, col-1});
        visited.insert({row, col-1});
    }
    if (validPos(row+1, col) && board[row+1][col] == -2 && visited.find({row+1, col}) == visited.end())
    {
        //cout << "QueueForVisiting row: " << row << " QueueForVisiting Col: " << col << endl;
        //cout << "Queue Pushing row: " << row+1 << " Queue Pushing Col: " << col << endl;
        ROTvisiting.push(Mines{0, row+1, col});
        visited.insert({row+1, col});
    }
    if (validPos(row-1, col) && board[row-1][col] == -2 && visited.find({row-1, col}) == visited.end())
    {
        //cout << "QueueForVisiting row: " << row << " QueueForVisiting Col: " << col << endl;
       // cout << "Queue Pushing row: " << row-1 << " Queue Pushing Col: " << col << endl;
        ROTvisiting.push(Mines{0, row-1, col});
        visited.insert({row-1, col});
    }
    if (validPos(row+1, col+1) && board[row+1][col+1] == -2 && visited.find({row+1, col+1}) == visited.end())
    {
       // cout << "QueueForVisiting row: " << row << " QueueForVisiting Col: " << col << endl;
        //cout << "Queue Pushing row: " << row+1 << " Queue Pushing Col: " << col+1 << endl;
        ROTvisiting.push(Mines{0, row+1, col+1});
        visited.insert({row+1, col+1});
    }
    if (validPos(row+1, col-1) && board[row+1][col-1] == -2 && visited.find({row+1, col-1}) == visited.end())
    {
       // cout << "QueueForVisiting row: " << row << " QueueForVisiting Col: " << col << endl;
       // cout << "Queue Pushing row: " << row+1 << " Queue Pushing Col: " << col-1 << endl;
        ROTvisiting.push(Mines{0, row+1, col-1});
        visited.insert({row+1, col-1});
    }
    if (validPos(row-1, col+1) && board[row-1][col+1] == -2 && visited.find({row-1, col+1}) == visited.end())
    {
       // cout << "QueueForVisiting row: " << row << " QueueForVisiting Col: " << col << endl;
       // cout << "Queue Pushing row: " << row-1 << " Queue Pushing Col: " << col+1 << endl;
        ROTvisiting.push(Mines{0, row-1, col+1});
        visited.insert({row+1, col-1});
    }
    if (validPos(row-1, col-1) && board[row-1][col-1] == -2 && visited.find({row-1, col-1}) == visited.end())
    {
       // cout << "QueueForVisiting row: " << row << " QueueForVisiting Col: " << col << endl;
       // cout << "Queue Pushing row: " << row-1 << " Queue Pushing Col: " << col-1 << endl;
        ROTvisiting.push(Mines{0, row-1, col-1});
        visited.insert({row-1, col-1});
    }
}


int MyAI::NumUnMarkedNeighbors(int row, int col)
{
    int count = 0;
    if (validPos(row, col+1))
    {
        if (board[row][col+1] == -2)
        {
            count++;
        }
    }

    if (validPos(row, col-1))
    {
        if (board[row][col-1] == -2)
        {
            count++;
        }
    }

    if (validPos(row+1, col))
    {
        if (board[row+1][col] == -2)
        {
            count++;
        }
    }

    if (validPos(row-1, col))
    {
        if (board[row-1][col] == -2)
        {
            count++;
        }
    }

    if (validPos(row-1, col+1))
    {
        if (board[row-1][col+1] == -2)
        {
            count++;
        }
    }

    if (validPos(row-1, col-1))
    {
        if (board[row-1][col-1] == -2)
        {
            count++;
        }
    }

    if (validPos(row+1, col+1))
    {
        if (board[row+1][col+1] == -2)
        {
            count++;
        }
    }

    if (validPos(row+1, col-1))
    {
        if (board[row+1][col-1] == -2)
        {
            count++;
        }
    }

    return count;
}

bool MyAI::validPos(int row, int col)
{
    if (row < 0 || col < 0 || row >= rowDimension || col >= rowDimension)
    {
        return false;
    }
    return true;
}

// ======================================================================
// YOUR CODE BEGINS
// ======================================================================



// ======================================================================
// YOUR CODE ENDS
// ======================================================================


/*
void MyAI::popClosest()
{
    remaining();
    poppingRest = true;
    int closestRow = 0;
    int closestCol = 0;
    int closestDistance = 10000;
    for (int i = 0; i < visiting.size(); ++i)
    {
        pair<int, int> temp = visiting.front();
        visiting.pop();
        visiting.push(temp);
        
        int distance = abs(sqrt(pow(temp.second - colDimension, 2) + pow(temp.first - rowDimension, 2)));

        if (distance < closestDistance)
        {
            closestDistance = distance;
            closestRow = temp.first;
            closestCol = temp.second;
        }
        else if (distance == closestDistance)
        {
            if ((rand() % 2) == 1)
            {
                closestDistance = distance;
                closestRow = temp.first;
                closestCol = temp.second;
            }
        }

    }

    for (int i = 0; i < visiting.size(); ++i)
    {
        pair<int, int> temp = visiting.front();
        visiting.pop();

        if (temp.first == closestRow && temp.second == closestCol)
        {
            break;
        }
        visiting.push(temp);
    }


}
*/