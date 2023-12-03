#include "SudokuCell.h"
#include "iostream"

SudokuCell::SudokuCell(int value) {
  this->value = value ;
  this->mutableCell = false;
  if(value == 0) {
    this->mutableCell = true;
  }
}

int SudokuCell::getValue() {
  return this->value;
}

bool SudokuCell::setValue(int value) {
  if(this->mutableCell) {
    this->value = value;
    return true;
  } else {
    return false;
  }
}

bool SudokuCell::getMutableCell() {
  return this->mutableCell;
}

