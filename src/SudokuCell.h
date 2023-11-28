#pragma once

class SudokuCell {
  private:
    int value;
    bool mutableCell;
  public:
    SudokuCell(int value);
    int getValue();
    bool setValue(int value);
    bool getMutableCell();
};
