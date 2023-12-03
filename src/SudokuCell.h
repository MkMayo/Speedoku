#include "vector"

class SudokuCell {
private:
    int value ;
    bool mutableCell;
    std::vector<int> candidates;  // New member to store candidates

public:
    SudokuCell(int value);
    int getValue();
    bool setValue(int value);
    bool getMutableCell();

};


