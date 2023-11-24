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
        std::vector<std::vector<char>> unsolved;
        std::vector<std::vector<char>> solved;

        SudokuBoard(const std::string& unsolvedStr, const std::string& solvedStr) {
            unsolved = stringToVector(unsolvedStr);
            solved = stringToVector(solvedStr);
        }

        std::vector<std::vector<char>> stringToVector(const std::string& str) {
            std::vector<std::vector<char>> result;

            for (int i = 0; i < 9; ++i) {
                result.push_back(std::vector<char>(str.begin() + i * 9, str.begin() + (i + 1) * 9));
            }

            return result;
        }
    };

    std::vector<SudokuBoard> sudokuBoards; // Vector to store multiple Sudoku boards
};

void drawSudokuBoard(sf::RenderWindow& window, const std::vector<std::vector<char>>& board) {
    sf::Font font;
    if (!font.loadFromFile("../arial.ttf")) {
        std::cerr << "Error loading font." << std::endl;
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(40); // Adjust the font size
    text.setFillColor(sf::Color::Black);

    float cellSize = 60.0f;
    float boldLineThickness = 9.0f; // Thickness of the bold lines

    // Draw bold lines around 3x3 squares
    sf::RectangleShape boldLine(sf::Vector2f(cellSize * 3, boldLineThickness));
    boldLine.setFillColor(sf::Color::Black);

    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            // Draw bold lines around 3x3 squares


            // Draw cell
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            cell.setFillColor(sf::Color::White);
            cell.setOutlineThickness(3.0f);
            cell.setOutlineColor(sf::Color::Black);
            cell.setPosition(static_cast<float>(j) * cellSize, static_cast<float>(i) * cellSize);
            window.draw(cell);

            // Draw number

                text.setString(board[i][j]);
                sf::FloatRect textRect = text.getLocalBounds();
                text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                text.setPosition(static_cast<float>(j) * cellSize + cellSize / 2.0f, static_cast<float>(i) * cellSize + cellSize / 2.0f);
                window.draw(text);

        }
    }
}


int main() {
    sf::RenderWindow window(sf::VideoMode(600, 600), "Sudoku Board");

    SudokuBoards sudokuBoards; // Create an instance of SudokuBoards to store multiple boards

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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::White);

        if (!sudokuBoards.sudokuBoards.empty()) {
            drawSudokuBoard(window, sudokuBoards.sudokuBoards[99999].unsolved);
        }

        window.display();
    }

    return 0;
}
