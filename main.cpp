#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;

class SudokuBoards {
public:
    class SudokuBoard {
    public:
        vector<std::vector<char>> unsolved;
        vector<std::vector<char>> solved;

        SudokuBoard(const string& unsolvedStr, const string& solvedStr) {
            unsolved = stringToVector(unsolvedStr);
            solved = stringToVector(solvedStr);
        }

        vector<std::vector<char>> stringToVector(const string& str) {
            vector<vector<char>> result;

            for (int i = 0; i < 9; ++i) {
                result.push_back(vector<char>(str.begin() + i * 9, str.begin() + (i + 1) * 9));
            }

            return result;
        }
    };

    vector<SudokuBoard> sudokuBoards; // vector to store multiple Sudoku boards
};

void drawSudokuBoard(sf::RenderWindow& window, const std::vector<std::vector<char>>& board, int currentRow, int currentCol) {
    sf::Font font;
    if (!font.loadFromFile("../arial.ttf")) {
        std::cerr << "Error loading font." << std::endl;
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(40); //font size
    text.setFillColor(sf::Color::Black);

    float cellSize = 60.0f;
    float boldLineThickness = 9.0f; // thickness of the bold lines

    sf::RectangleShape boldLine(sf::Vector2f(cellSize * 3, boldLineThickness));
    boldLine.setFillColor(sf::Color::Black);

    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {

            // draw cell
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            cell.setFillColor(sf::Color::White);
            cell.setOutlineThickness(3.0f);
            cell.setOutlineColor(sf::Color::Black);
            cell.setPosition(static_cast<float>(j) * cellSize, static_cast<float>(i) * cellSize);
            window.draw(cell);

            // draw number
            text.setString(board[i][j]);
            if (i < currentRow || (i == currentRow && j <= currentCol)) {
                // draw solved numbers in black
                text.setFillColor(sf::Color::Black);
            } else {
                // draw current solving numbers in red
                text.setFillColor(sf::Color::Red);
            }

            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
            text.setPosition(static_cast<float>(j) * cellSize + cellSize / 2.0f, static_cast<float>(i) * cellSize + cellSize / 2.0f);
            window.draw(text);
        }
    }
}

// Check if it's safe to place the digit at board[row][col]
bool isSafe(vector<vector<char>>& board, int row, int col, char digit) {
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == digit || board[i][col] == digit) {
            return false;
        }
    }

    int subgridStartRow = 3 * (row / 3);
    int subgridStartCol = 3 * (col / 3);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[subgridStartRow + i][subgridStartCol + j] == digit) {
                return false;
            }
        }
    }

    return true;
}

// Solve the Sudoku using backtracking
bool solveSudoku(sf::RenderWindow& window, vector<vector<char>>& board, int row, int col) {
    if (row == 9) {
        return true;
    }

    if (col == 9) {
        return solveSudoku(window, board, row + 1, 0);
    }

    if (board[row][col] == '0' || board[row][col] == ' ') {

        for (char digit = '1'; digit <= '9'; ++digit) {
            if (isSafe(board, row, col, digit)) {
                board[row][col] = digit;
                drawSudokuBoard(window, board, row, col);
                window.display();
                sf::sleep(sf::milliseconds(100));

                if (solveSudoku(window, board, row, col + 1)) {
                    return true;
                }
                board[row][col] = '0';
            }
        }
        return false;
    } else {
        return solveSudoku(window, board, row, col + 1);
    }
}

void printBoard(const vector<vector<char>>& board) {
    for (const auto& row : board) {
        for (char cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 600), "Sudoku Board");

    SudokuBoards sudokuBoards; // create an instance of SudokuBoards to store multiple boards

    ifstream inputFile("../sudokuNew.csv");

    if (!inputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return 1;
    }

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

    window.clear(sf::Color::White);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (!sudokuBoards.sudokuBoards.empty()) {
            int boardIndex = 1;
            vector<vector<char>> unsolvedBoard = sudokuBoards.sudokuBoards[boardIndex].unsolved;

            // print the unsolved board for debugging
            cout << "Unsolved Board:" << endl;
            printBoard(unsolvedBoard);

            vector<vector<char>> solvedBoard = unsolvedBoard;  // create a copy of the unsolved board


            for (int row = 0; row < 9; ++row) {
                for (int col = 0; col < 9; ++col) {
                    solveSudoku(window, solvedBoard, col, row);
                    drawSudokuBoard(window, solvedBoard, row, col);
                    window.display();
                    sf::sleep(sf::milliseconds(100)); // adjust the delay as needed
                }
            }
        }
    }



    return 0;
}
