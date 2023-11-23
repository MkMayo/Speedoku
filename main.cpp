#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class SudokuBoards {
public:
    class SudokuBoard {
    public:
        vector<vector<char>> unsolved;
        vector<vector<char>> solved;

        SudokuBoard(string unsolvedStr, string solvedStr) {
            unsolved = stringToVector(unsolvedStr);
            solved = stringToVector(solvedStr);
        }

        vector<vector<char>> stringToVector(string str) {
            vector<vector<char>> result;

            for (int i = 0; i < 9; ++i) {
                result.push_back(vector<char>(str.begin() + i * 9, str.begin() + (i + 1) * 9));
            }

            return result;
        }
    };

    vector<SudokuBoard> sudokuBoards;

};

void printBoard(vector<vector<char>> board) {
    for (auto row : board) {
        for (char cell : row) {
            cout << cell << " ";
        }
        cout << std::endl;
    }

    cout << endl;
}

int main() {
    ifstream inputFile("../sudokuNew.csv");

    if (!inputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return 1;
    }

    SudokuBoards sudokuBoards;

    string line;
    while (getline(inputFile, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            string unsolved = line.substr(0, commaPos);
            string solved = line.substr(commaPos + 1);

            SudokuBoards::SudokuBoard sudokuBoard(unsolved, solved);
            sudokuBoards.sudokuBoards.push_back(sudokuBoard);

            //cout << "Unsolved:" << std::endl;
            //printBoard(sudokuBoard.unsolved);

            //cout << "Solved:" << std::endl;
            //printBoard(sudokuBoard.solved);
        } else {
            std::cerr << "Invalid line format in CSV file." << std::endl;
        }
    }

    if (!sudokuBoards.sudokuBoards.empty()) {
        // Garbage values on sudokuBoards.sudokuBoards[0] so start from 1
        cout << "Unsolved Board 1:" << std::endl;
        printBoard(sudokuBoards.sudokuBoards[1].unsolved);

        cout << "Solved Board 1:" << std::endl;
        printBoard(sudokuBoards.sudokuBoards[1].solved);
    }

    inputFile.close();

    return 0;
}



