#include <algorithm> 
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

/*
using std::vector;
using std::string;
using std::cout;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::abs;
*/

/*
    🚦   ⛰️   0    0    0    0
    🚗   ⛰️   0    0    0    0
    🚗   ⛰️   0    0    0    0
    🚗   ⛰️   0    🚗   🚗   🚗
    🚗   🚗   🚗   🚗   ⛰️   🏁
*/
enum class State { kEmpty, kObstacle, kClosed, kPath, kStart, kFinish };

vector<State> ParseLine(string line) {
    istringstream sline(line);

    int n;
    char c;

    vector<State> row;

    while(sline >> n >> c && c == ',') {
        if (n == 0) {
            row.push_back(State::kEmpty);
        } else {
            row.push_back(State::kObstacle);
        }
    }

    return row;
}

/**
 *  Add a node to the open list and mark it as open
 */
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &grid) {
    // Add node to open vector, and mark grid cell as closed.
    openlist.push_back(vector<int>{x, y, g, h});
    grid[x][y] = State::kClosed;
}

vector<vector<State>> ReadBoardFile(string path) {
    ifstream myfile(path);
    vector<vector<State>> board{};
    if (myfile) {
        string line;

        while(getline(myfile, line)) {
            vector<State> row = ParseLine(line);
            board.push_back(row);
        }
    }
    return board;
}

/*
 * Compare the F values of two cells
 */
bool Compare(const vector<int> a, const vector<int> b) {
    int f1 = a[2] + a[3];
    int f2 = b[2] + b[3];

    return f1 > f2;
}

// *.c          <- Nur bei C     -> Code File
// *.cpp        <- Nur bei C++   -> Code File
// *.h          Header File für C
// *.hpp        Header File für C++

// const
// constexpr

void CellSort(vector<vector<int>> *v) {
    sort(v->begin(), v->end(), Compare);
}

// Calculate the manhattan distance
int Heuristic(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}

/*
    Check that a cell is valid: on the grid, not an obstacle and clear
*/
bool CheckValidCell (int x, int y, vector<vector<State>> &grid) {
    bool on_grid_x = (x >= 0 && x < grid.size());
    bool on_grid_y = (y >= 0 && y < grid[0].size());

    if (on_grid_x && on_grid_y) {
        return grid[x][y] == State::kEmpty;
    }

    return false;
}

// directional deltas
const int delta[4][2]{{-1,0}, {0,-1}, {1,0}, {0,1}};
//
//                 |
//                 |
//                 |
//              *  | *
//  ---------------o---------------
//              *  | *
//                 |
//                 |
//                 |

void ExpandNeighbors(const vector<int> &current, int goal[2], 
                     vector<vector<int>> &openlist, vector<vector<State>> &grid) {
    // Get current node's data.
    int x = current[0];
    int y = current[1];
    int g = current[2];

    //             up
    //             |
    //      left --o-- right
    //             |
    //           dowwn 
    //
    // Loop through current node's potential neighbors.
    for (int i = 0; i < 4; i++) {
        int x2 = x + delta[i][0];
        int y2 = y + delta[i][0];
        // Check that the potential neighbor's x2 and y2 values are on the grid and not closed
        if (CheckValidCell(x2, y2, grid)) {
            // Increment g value, compute h value, and add neighbor to open list
            int g2 = g + 1;
            int h2 = Heuristic(x2, y2, goal[0], goal[1]);
            AddToOpen(x2, y2, g2, h2, openlist, grid);
        }
    }
}

vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2]) {
    // Crate the vector of open nodes.
    vector<vector<int>> open {};

    // Initialize the starting node.
    int x = init[0];
    int y = init[1];
    int g = 0;
    int h  = Heuristic(x, y, goal[0], goal[1]);
    AddToOpen(x, y, g, h, open, grid);

    while(open.size() > 0) {
        // Sort the nodes
        CellSort(&open);

        auto current = open.back();
        open.pop_back();

        x = current[0];
        y = current[1];

        grid[x][y] = State::kPath;

        // Check if we are done ...
        if (x == goal[0] && y == goal[1]) {
            grid[init[0]][init[1]] = State::kStart;
            grid[goal[0]][goal[1]] = State::kFinish;
            return grid;
        }

        // If we are not done, expand search to current node's neighbors.
        ExpandNeighbors(current, goal, open, grid);
    }

    cout << "No path found!" << "\n";
    return vector<vector<State>> {};
}

string CellString(State cell) {
    switch(cell) {
        case State::kObstacle : return "⛰️   ";
        case State::kPath : return "🚗   ";
        case State::kStart : return "🚦   ";
        case State::kFinish : return "🏁   ";
        default : return "0   ";
    }
}

// TODO: Create the CellString function here,
//       using the following return strings:
// "⛰️   "
// "0   "
/*
Sample output:
0   ⛰️   0   0   0   0
0   ⛰️   0   0   0   0
0   ⛰️   0   0   0   0
0   ⛰️   0   0   0   0
0   0    0   0  ⛰️   0
*/
void PrintBoard(const vector<vector<State>> board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            cout << CellString(board[i][j]);
        }
        cout << "\n";
    }
}
