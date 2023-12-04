#pragma once

#include <string>
#include <vector>
#include "SudokuCell.h"
using namespace std;

class SudokuBoard {
private:
    vector<SudokuCell> board;

public:
    SudokuBoard(string rawBoardStr);
    int getValAtIndex(int index);
    void debugPrintBoard();
    bool isSafeRow(int num, int row);
    bool isSafeCol(int num, int col);
    bool isSafeSquare(int num, int row, int col);
    bool isSafe(int num, int row, int col);
    vector<int> findNext();
    vector<pair<int, int>> findMutables();
    int getValAtCoords(int row, int col);
    bool place(int num, int row, int col);
    std::vector<int> getCandidates(int row, int col) ;
    bool isSolved();
};
