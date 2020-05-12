#include "test.cpp"

int main() {
    int init[2] {0,0};
    int goal[2] {4,5};

    /*
        🚦   ⛰️   0    0    0    0
        🚗   ⛰️   0    0    0    0
        🚗   ⛰️   0    0    0    0
        🚗   ⛰️   0    🚗   🚗   🚗
        🚗   🚗   🚗   🚗   ⛰️   🏁
    */
    auto board = ReadBoardFile("1.board");
    auto solution = Search(board, init, goal);
    PrintBoard(solution);

    // Tests
    TestHeuristic();
    TestAddToOpen();
    TestCompare();
    TestSearch();
    TestCheckValidCell();
    TestExpandNeighbors();

    /*
    int i;
    cout << "Enter an integer value for i: ";
    cin >> i;

    // j can only be evaluated at run time.
    // But I promise not to change this value after i is initialized
    const int j = 2 * i;

    // k, in contrast, can be evaluated at compile
    // "to be evaluated at compile time"
    constexpr int k = 3;

    cout << "j = " << j << endl;
    cout << "k = " << k << endl;
    */
}