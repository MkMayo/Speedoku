#include <string>
#include <vector>
#include "SudokuBoard.h"
using namespace std;

#include <iostream>

// Converts coordinates to an index in the vector
int findIndexFromCoords(int row, int col) {
  return (row * 9) + col;
}

// Initializes a board given a string of digits
SudokuBoard::SudokuBoard(string rawBoardStr) {
  vector<SudokuCell> board;

  for(char c : rawBoardStr) {
    board.push_back(SudokuCell(stoi(string(1, c))));
  }

  this->board = board;
}

// Prints a board nicely on CLI
void SudokuBoard::debugPrintBoard() {
  for(unsigned int i = 0; i < 81; i++) {
    if(i != 0 && i % 9 == 0) {
      cout << endl;
      for(unsigned int j = 0; j < 33; j++) {
        if(j % 4 != 2) {
          cout << "-";
        } else {
          cout << "+";
        }
      }
      cout << endl;
    }
    if(i % 9 != 0) {
      cout << " | ";
    }
    if(this->board[i].getValue() != 0) {
      cout << this->board[i].getValue();
    } else {
      cout << " ";
    }
  }
  cout << endl;
}

// Checks if a given row can accept a certain number
bool SudokuBoard::isSafeRow(int num, int row) {
  for(unsigned int i = 0; i < 9; i++) {
    if(this->board[findIndexFromCoords(row, i)].getValue() == num) {
      return false;
    }
  }
  return true;
}

// Checks if a given col can accept a certain number
bool SudokuBoard::isSafeCol(int num, int col) {
  for(unsigned int i = 0; i < 9; i++) {
    if(this->board[findIndexFromCoords(i, col)].getValue() == num) {
      return false;
    }
  }
  return true;
}

// Checks if a given square can accept a certain number
bool SudokuBoard::isSafeSquare(int num, int row, int col) {
  int squareRowStart = (row / 3) * 3;
  int squareColStart = (col / 3) * 3;
  for(unsigned int i = 0; i < 3; i++) {
    for(unsigned int j = 0; j < 3; j++) {
      if(this->board[findIndexFromCoords(squareRowStart + i, squareColStart + j)].getValue() == num) {
        return false;
      }
    }
  }
  return true;
}

// Checks if a given tile can accept a certain number
bool SudokuBoard::isSafe(int num, int row, int col) {
  return (this->isSafeRow(num, row) && this->isSafeCol(num, col) && this->isSafeSquare(num, row, col));
}

// Finds the next empty tile
vector<int> SudokuBoard::findNext() {
  int index = -1;
  for(unsigned int i = 0; i < this->board.size(); i++) {
    if(board[i].getValue() == 0) {
      index = i;
      break;
    }
  }
  vector<int> out;
  if(index != -1) {
    out.push_back(index / 9);
    out.push_back(index % 9);
  }
  return out;
}

// Returns a vector of integer pairs representing coordinates
// of all mutable tiles on the board
vector<pair<int, int>> SudokuBoard::findMutables() {
  vector<pair<int, int>> out;
  for(unsigned int i = 0; i < this->board.size(); i++) {
    if(this->board[i].getMutableCell()) {
      out.push_back(make_pair(i / 9, i % 9));
    }
  }
  return out;
}

// Returns the value of the cell at the coordinates
int SudokuBoard::getValAtCoords(int row, int col) {
  return this->board[findIndexFromCoords(row, col)].getValue();
}

// Places a number on a tile
bool SudokuBoard::place(int num, int row, int col) {
  return this->board[findIndexFromCoords(row, col)].setValue(num);
}
