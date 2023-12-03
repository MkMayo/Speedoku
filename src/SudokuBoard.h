#pragma once

#include <string>
#include <vector>
#include "SudokuCell.h"
using namespace std;

class SudokuBoard {
  private:
    vector<SudokuCell> board;
    static const int ROW_UNIT = 0;
    static const int COL_UNIT = 1;
    static const int BOX_UNIT = 2;

  public:
    SudokuBoard(string rawBoardStr);
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
