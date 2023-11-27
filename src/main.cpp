#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
using namespace std;

class TextureManager{
public:
    unordered_map<string, sf::Texture*> texture_map;
    TextureManager(){
        sf::Texture* texture_1 = new sf::Texture;
        texture_map.emplace("icon", texture_1);
        texture_map["icon"]->loadFromFile("../../assets/sudoku_icon.png");

        sf::Texture* texture_2 = new sf::Texture;
        texture_map.emplace("speedoku_text", texture_2);
        texture_map["speedoku_text"]->loadFromFile("../../assets/speedoku_text.png");

        sf::Texture* texture_3 = new sf::Texture;
        texture_map.emplace("start_icon", texture_3);
        texture_map["start_icon"]->loadFromFile("../../assets/start_icon.png");
    }
};


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

    if (!font.loadFromFile("../../assets/arial.ttf")) {
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
//clone of other board function to test multiple boards

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


    sf::RenderWindow welcome_window(sf::VideoMode(800, 800), "Welcome");
    welcome_window.clear(sf::Color::White);

    TextureManager manager; //texture manager object

    sf::Sprite icon;
    icon.setTexture(*manager.texture_map["icon"]);
    auto texture_temp = *manager.texture_map["icon"];
    icon.setOrigin(sf::Vector2f(texture_temp.getSize().x / 2, texture_temp.getSize().y / 2));
    icon.setScale(0.65f, 0.65f);
    icon.setPosition(400, 375);



    sf::Sprite speedoku_text;
    speedoku_text.setTexture(*manager.texture_map["speedoku_text"]);
    texture_temp = *manager.texture_map["speedoku_text"];
    speedoku_text.setOrigin(sf::Vector2f(texture_temp.getSize().x / 2, texture_temp.getSize().y / 2));
    speedoku_text.setScale(1.25f, 1.25f);
    speedoku_text.setPosition(400, 125);


    sf::Sprite start_icon;
    start_icon.setTexture(*manager.texture_map["start_icon"]);
    texture_temp = *manager.texture_map["start_icon"];
    start_icon.setOrigin(sf::Vector2f(texture_temp.getSize().x / 2, texture_temp.getSize().y / 2));
    start_icon.setScale(0.65f, 0.65f);
    start_icon.setPosition(400, 675);



    while(welcome_window.isOpen()){ //welcome window loop

        sf::Event event;
        while (welcome_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                welcome_window.close();
            }
        }
        if(event.type == sf::Event::MouseButtonPressed){
            sf::Mouse mouse;
            bool left;
            auto coordinate = mouse.getPosition(welcome_window);

            int x_size = start_icon.getTexture()->getSize().x * start_icon.getScale().x;
            int y_size = start_icon.getTexture()->getSize().y * start_icon.getScale().y;
            int y_position = start_icon.getPosition().y;
            if(coordinate.x >= 400 - (x_size / 2) and coordinate.x <= 400 + (x_size / 2)){ //if start button pressed
                if(coordinate.y >= y_position - (y_size / 4) and coordinate.y <= y_position + (y_size / 4)){
                    welcome_window.close();
                }
            }
        }

        welcome_window.clear(sf::Color::White);
        welcome_window.draw(icon);
        welcome_window.draw(start_icon);
        welcome_window.draw(speedoku_text);
        welcome_window.display();
    }

    sf::RenderWindow window(sf::VideoMode(1080, 1080), "Sudoku Board");
    window.clear(sf::Color::White);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(); //keeps board from flashing
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
                    sf::sleep(sf::milliseconds(100)); // adjust the delay as neede
                }
            }
        }
    }



    return 0;
}
