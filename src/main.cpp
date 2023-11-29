#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <tuple>
#include "SudokuBoard.h"
using namespace std;

// Function to load a single texture
void loadTexture(const string& textureName, map<string, sf::Texture>& textures) {
    sf::Texture texture;
    textures.emplace(textureName, texture);
    textures[textureName].loadFromFile("assets/" + textureName + ".png");
}

// Function to initialize all textures
map<string, sf::Texture> initializeTextures() {
    map<string, sf::Texture> textures;
    
    loadTexture("sudoku_icon", textures);
    loadTexture("speedoku_text", textures);
    loadTexture("start_icon", textures);

    return textures;
}

// Function to initialize all Sudoku boards in the dataset
vector<SudokuBoard> initializeBoards() {
    vector<SudokuBoard> boards;
    
    ifstream inputFile("assets/dataset.csv");

    if(!inputFile.is_open()) {
        cerr << "Error opening the file." << endl;
        return boards;
    }

    string line;
    while(getline(inputFile, line)) {
        int commaPos = line.find(',');
        if(commaPos != string::npos) {
            string unsolved = line.substr(0, commaPos);
            if(unsolved.size() == 81) {
                SudokuBoard board(unsolved);
                boards.push_back(board);
            }
        } else {
            std::cerr << "Invalid line format in CSV file." << std::endl;
        }
    }

    return boards;
}

// Merge for merge sort
vector<tuple<int, int, vector<int>>> merge(vector<tuple<int, int, vector<int>>> left, vector<tuple<int, int, vector<int>>> right) {
    unsigned int li = 0;
    unsigned int ri = 0;
    vector<tuple<int, int, vector<int>>> out;
    while(li < left.size() || ri < right.size()) {
        if(li == left.size()) {
            for(unsigned int i = ri; i < right.size(); i++) {
                out.push_back(right[i]);
            }
            break;
        } else if(ri == right.size()) {
            for(unsigned int i = li; i < left.size(); i++) {
                out.push_back(left[i]);
            }
            break;
        } else if(get<2>(left[li]).size() < get<2>(right[ri]).size()) {
            out.push_back(left[li]);
            li++;
        } else {
            out.push_back(right[ri]);
            ri++;
        }
    }
    return out;
}

// Merge sort for Cross-Hatching approach
vector<tuple<int, int, vector<int>>> mergeSort(vector<tuple<int, int, vector<int>>> vec) {
    if(vec.size() <= 1) {
        return vec;
    }
    unsigned int halfPoint = 0;
    if(vec.size() % 2 == 0) {
        halfPoint = vec.size() / 2;
    } else {
        halfPoint = (vec.size() - 1) / 2;
    }
    vector<tuple<int, int, vector<int>>> left;
    vector<tuple<int, int, vector<int>>> right;
    for(unsigned int i = 0; i < vec.size(); i++) {
        if(i < halfPoint) {
            left.push_back(vec[i]);
        } else {
            right.push_back(vec[i]);
        }
    }
    left = mergeSort(left);
    right = mergeSort(right);
    return merge(left, right);
}

void drawSudokuBoard(SudokuBoard board, sf::RenderWindow& window, int algo) {
    sf::Font font;

    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Error loading font." << std::endl;
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(36); //font size
    text.setFillColor(sf::Color::Black);

    float cellSize = 48.0f;
    float borderSize = 8.0f;

    sf::RectangleShape background(sf::Vector2f((10 * borderSize) + (9 * cellSize), (10 * borderSize) + (9 * cellSize)));
    background.setOrigin(sf::Vector2f(((10 * borderSize) + (9 * cellSize)) / 2, ((10 * borderSize) + (9 * cellSize)) / 2));
    background.setFillColor(sf::Color::Black);
    background.setPosition(640, 320);

    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
    cell.setOrigin(sf::Vector2f(cellSize / 2, cellSize / 2));
    cell.setFillColor(sf::Color::White);

    window.clear(sf::Color::White);
    window.draw(background);
    float initPosX = 640 - ((cellSize + borderSize) * 4);
    float initPosY = 320 - ((cellSize + borderSize) * 4);
    float posX = initPosX;
    float posY = initPosY;
    for(unsigned int i = 0; i < 9; i++) {
        for(unsigned int j = 0; j < 9; j++) {
            cell.setPosition(posX, posY);
            window.draw(cell);
            if(board.getValAtCoords(i, j) != 0) {
                text.setString(to_string(board.getValAtCoords(i, j)));
            } else {
                text.setString("");
            }
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + (textRect.width / 2.0f), textRect.top + (textRect.height / 2.0f));
            text.setPosition(posX, posY);
            window.draw(text);
            posX += cellSize + borderSize;
        }
        posX = initPosX;
        posY += cellSize + borderSize;
    }
    window.display();
}
//clone of other board function to test multiple boards

// Solves the board using Backtracking
bool solveBacktracking(SudokuBoard& board, sf::RenderWindow& window) {
    vector<int> coords = board.findNext();
    if(coords.empty()) return true;
    for(unsigned int i = 1; i <= 9; i++) {
        if(board.isSafe(i, coords[0], coords[1])) {
            board.place(i, coords[0], coords[1]);
            // PLACE DRAW FUNC HERE
            drawSudokuBoard(board, window, 0);
            if(solveBacktracking(board, window)) return true;
            board.place(0, coords[0], coords[1]);
        }
    }
    return false;
}

// Solves the board using Cross-Hatching
bool solveCrossHatch(SudokuBoard& board, sf::RenderWindow& window) {
    vector<pair<int, int>> mutables = board.findMutables();
    vector<tuple<int, int, vector<int>>> options;
    for(pair<int, int> c : mutables) {
        if(board.getValAtCoords(c.first, c.second) != 0) continue;
        vector<int> cellOptions;
        for(unsigned int i = 1; i <= 9; i++) {
            if(board.isSafe(i, c.first, c.second)) cellOptions.push_back(i);
        }
        options.push_back(make_tuple(c.first, c.second, cellOptions));
    }
    if(options.size() == 0) return true;
    options = mergeSort(options);
    for(int i : get<2>(options[0])) {
        board.place(i, get<0>(options[0]), get<1>(options[0]));
        // PLACE DRAW FUNC HERE
        drawSudokuBoard(board, window, 1);
        if(solveCrossHatch(board, window)) return true;
        board.place(0, get<0>(options[0]), get<1>(options[0]));
    }
    return false;
}

// // Solve the Sudoku using backtracking
// bool solveSudoku(sf::RenderWindow& window, vector<vector<char>>& board, int row, int col) {
//     if (row == 9) {
//         return true;
//     }

//     if (col == 9) {
//         return solveSudoku(window, board, row + 1, 0);
//     }

//     if (board[row][col] == '0' || board[row][col] == ' ') {

//         for (char digit = '1'; digit <= '9'; ++digit) {
//             if (isSafe(board, row, col, digit)) {
//                 board[row][col] = digit;
//                 drawSudokuBoard(window, board, row, col);
//                 window.display();
//                 sf::sleep(sf::milliseconds(100));

//                 if (solveSudoku(window, board, row, col + 1)) {
//                     return true;
//                 }
//                 board[row][col] = '0';
//             }
//         }
//         return false;
//     } else {
//         return solveSudoku(window, board, row, col + 1);
//     }
// }

int main() {
    vector<SudokuBoard> boards = initializeBoards();
    map<string, sf::Texture> textures = initializeTextures();

    sf::RenderWindow welcome_window(sf::VideoMode(1280, 720), "Speedoku");
    welcome_window.clear(sf::Color::White);

    sf::Sprite sudoku_icon;
    sudoku_icon.setTexture(textures["sudoku_icon"]);
    sudoku_icon.setOrigin(sf::Vector2f(textures["sudoku_icon"].getSize().x / 2, textures["sudoku_icon"].getSize().y / 2));
    sudoku_icon.setScale(1.0f, 1.0f);
    sudoku_icon.setPosition(640, 200);

    sf::Sprite speedoku_text;
    speedoku_text.setTexture(textures["speedoku_text"]);
    speedoku_text.setOrigin(sf::Vector2f(textures["speedoku_text"].getSize().x / 2, textures["speedoku_text"].getSize().y / 2));
    speedoku_text.setScale(1.0f, 1.0f);
    speedoku_text.setPosition(640, 120);

    sf::Sprite start_icon;
    start_icon.setTexture(textures["start_icon"]);
    start_icon.setOrigin(sf::Vector2f(textures["start_icon"].getSize().x / 2, textures["start_icon"].getSize().y / 2));
    start_icon.setScale(0.65f, 0.65f);
    start_icon.setPosition(640, 480);

    int mode = 0;
    int board = 0;
    while(welcome_window.isOpen()){ //welcome window loop
        sf::Event event;
        while (welcome_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                welcome_window.close();
            } else {
                if(mode == 0) {
                    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left){
                        int x_size = start_icon.getTexture()->getSize().x * start_icon.getScale().x;
                        int y_size = start_icon.getTexture()->getSize().y * start_icon.getScale().y;
                        int x_position = start_icon.getPosition().x;
                        int y_position = start_icon.getPosition().y;
                        if(event.mouseButton.x >= x_position - (x_size / 2) && event.mouseButton.x <= x_position + (x_size / 2) && event.mouseButton.y >= y_position - (y_size / 2) && event.mouseButton.y <= y_position + (y_size / 2)) {
                            mode = 1;
                        }
                    }
                }
            }
        }

        if(mode == 0) {
            welcome_window.clear(sf::Color::White);
            welcome_window.draw(sudoku_icon);
            welcome_window.draw(start_icon);
            welcome_window.draw(speedoku_text);
            welcome_window.display();
        } else if(mode == 1) {
            // TODO: Implement solve screens.
            drawSudokuBoard(boards[board], welcome_window, 0);
            sf::sleep(sf::milliseconds(2500));
            SudokuBoard boardAlgo1 = boards[board];
            solveBacktracking(boardAlgo1, welcome_window);
            sf::sleep(sf::milliseconds(2500));
            drawSudokuBoard(boards[board], welcome_window, 0);
            sf::sleep(sf::milliseconds(2500));
            SudokuBoard boardAlgo2 = boards[board];
            solveCrossHatch(boardAlgo2, welcome_window);
            welcome_window.close();
        } else if(mode == 2) {
            // TODO: Implement post-solve time comparison screen.
        }
    }
    
    return 0;
}

