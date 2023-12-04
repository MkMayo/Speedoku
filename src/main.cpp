#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <tuple>
#include <chrono>
#include "SudokuBoard.h"
#include "AlgorithmManager.h"
#include <unordered_map>

using namespace std ;

// Function to load a single texture
void loadTexture(const string& textureName, map<string, sf::Texture>& textures) {
    sf::Texture texture;
    textures.emplace(textureName, texture);
    textures[textureName].loadFromFile("../../assets/" + textureName + ".png");
}

// Function to initialize all texturesd
map<string, sf::Texture> initializeTextures() {
    map<string, sf::Texture> textures;

    loadTexture("sudoku_icon", textures);
    loadTexture("speedoku_text", textures);
    loadTexture("start_icon", textures);
    loadTexture("comparisonsOLD", textures);
    loadTexture("Speedoku", textures);
    loadTexture("comparisons", textures);

    return textures;
}

// Function to initialize all Sudoku boards in the dataset
vector<SudokuBoard> initializeBoards() {
    vector<SudokuBoard> boards;

    ifstream inputFile("../../assets/dataset.csv");

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

    if (!font.loadFromFile("../../assets/arial.ttf")) {
        std::cerr << "Error loading font." << std::endl;
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(36); //font size
    text.setFillColor(sf::Color::Black);

    sf::Text enterText;
    enterText.setFont(font);
    enterText.setCharacterSize(48);
    enterText.setFillColor(sf::Color::Black);
    enterText.setString("Press Enter To See Algorithm Stats!");

    sf::Text algoText;
    algoText.setFont(font);
    algoText.setCharacterSize(36); // Adjust the font size as needed
    algoText.setFillColor(sf::Color::Black);

    if (algo == 0){
        algoText.setString("Backtracking");
    }
    else if (algo == 1){
        algoText.setString("Cross-Hatching");
    }
    else if (algo == 2){
        algoText.setString("Naked Singles");
    }
    else if (algo == 3){
        algoText.setString("Hidden Singles with Backtracking");
    }


    float cellSize = 48.0f;
    float borderSize = 8.0f;

    sf::RectangleShape background(sf::Vector2f((10 * borderSize) + (9 * cellSize), (10 * borderSize) + (9 * cellSize)));
    background.setOrigin(sf::Vector2f(((10 * borderSize) + (9 * cellSize)) / 2, ((10 * borderSize) + (9 * cellSize)) / 2));
    background.setFillColor(sf::Color::Black);
    background.setPosition(640, 420);

    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
    cell.setOrigin(sf::Vector2f(cellSize / 2, cellSize / 2));
    cell.setFillColor(sf::Color::White);

    window.clear(sf::Color::White);
    window.draw(background);
    float initPosX = 640 - ((cellSize + borderSize) * 4);
    float initPosY = 420 - ((cellSize + borderSize) * 4);
    float posX = initPosX;
    float posY = initPosY;

    sf::FloatRect textBounds = algoText.getLocalBounds();
    algoText.setPosition(640 - textBounds.width / 2, 100); // Adjust the position based on your layout

    sf::FloatRect textBounds1 = enterText.getLocalBounds();
    enterText.setPosition(640 - textBounds1.width/2, 10); // Adjust the position based on your layout

    window.draw(algoText);
    window.draw(enterText);


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
bool solveBacktracking(SudokuBoard& board, sf::RenderWindow& window, bool singles) {
    vector<int> coords = board.findNext();

    if(coords.empty()) return true;
    for(unsigned int i = 1; i <= 9; i++) {
        if(board.isSafe(i, coords[0], coords[1])) {
            board.place(i, coords[0], coords[1]);
            // PLACE DRAW FUNC HERE
            if(!singles){drawSudokuBoard(board, window, 0);}
            else{
                drawSudokuBoard(board, window, 3);
            }
            if(solveBacktracking(board, window, singles)) return true;
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
        drawSudokuBoard(board, window, 1);
        if(solveCrossHatch(board, window)) return true;
        board.place(0, get<0>(options[0]), get<1>(options[0]));
    }
    return false;
}

// Function to find and fill naked singles in the Sudoku board
bool findNakedSingle(SudokuBoard& board, sf::RenderWindow& window) {
    bool foundSingle = false;
    // Iterate through each cell in the board
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            // Check if the cell is empty
            if (board.getValAtCoords(row, col) == 0) {
                // Get the candidates for the empty cell
                vector<int> candidates = board.getCandidates(row, col);
                // If there's only one candidate, place it on the board
                if (candidates.size() == 1) {
                    board.place(candidates[0], row, col);
                    // Draw the updated board (assuming drawSudokuBoard is defined)
                    drawSudokuBoard(board, window, 2);
                    foundSingle = true; // Mark that a single was found
                }
            }
        }
    }

    return foundSingle; // Return whether a single was found in this iteration
}

// Function to solve the Sudoku board using naked singles and crosshatching
bool solveNakedSingle(SudokuBoard& board, sf::RenderWindow& window) {
    // Continue solving until the board is solved
    while (true) {
        // Try to find and fill naked singles
        if (!findNakedSingle(board, window)) {
            // If no naked singles are found, move on to crosshatching strategy
            solveCrossHatch(board, window);
            break; // Break out of the loop once crosshatching is done
        }
    }
    return board.isSolved(); // Return whether the board is solved
}


bool solveHiddenSingles(SudokuBoard& board){
    //each cell by default has values that could go there
    //if a cell in a box has an option that no other cell in the box has, then that option must go there
    //look at each box

    for (int r = 0; r < 9; r += 3) { // iterate through each box (row)
        for (int c = 0; c < 9; c += 3) { // iterate through each box (column)
            std::vector<int> box_index; // vector to store indices of each cell in the current box

            for (int i = r; i < r + 3; ++i) { // iterate through each cell in the box (row)
                for (int j = c; j < c + 3; ++j) { // iterate through each cell in the box (column)
                    int index = i * 9 + j;
                    box_index.push_back(index);
                }
            }
            std::vector<int> arrayOfVectors[9];
            int count = 0;
            for(int cell_index : box_index) {
                vector<int> temp_possibles;
                if(board.getValAtIndex(cell_index) == 0){ //if mutable
                    //push into array of vectors a vector containing the values each cell could be
                    for(int i = 1; i < 10; ++i){
                        int row_temp = cell_index / 9;
                        int col_temp = cell_index % 9;
                        if(board.isSafe(i, row_temp, col_temp)){ //if digit could go in the cell given by the index
                            temp_possibles.push_back(i);
                        }
                    }
                    arrayOfVectors[count] = temp_possibles;
                }
                count += 1;
            }
            vector<int> found_singles;
            unordered_map<int, int> number_possibles;

            //initalize maps with possible digits and the number of cells with that possible digit
            for(int z = 1; z < 10; z++) {
                auto temp_pair = make_pair(z, 0);
                number_possibles.insert(temp_pair);
            }

            for(int i = 0; i < 9; ++i){ //for each value in the box
                if(!arrayOfVectors[i].empty()){ //if mutable
                    for(int x = 0; x < arrayOfVectors[i].size(); ++x){ //for each possible value in the cell dictated by i
                        number_possibles[arrayOfVectors[i].at(x)] += 1; //update that values count in the map
                    }
                }
            }

            //go through the map, if any value has a 1 associated that value is a hidden single in the box
            for(int i = 1; i < 10; ++i){
                if(number_possibles[i] == 1){
                    found_singles.push_back(i);
                }
            }

            //we now have a vector containing the hidden singles for this box
            for(int cell_index : box_index){
                for(int i : found_singles){
                    int row_temp = cell_index / 9;
                    int col_temp = cell_index % 9;
                    if(board.isSafe(i, row_temp, col_temp)){ //if digit could go in the cell given by the index
                        //at this point i is the single and row_temp and col_temp represent the coords of where it should go
                        board.place(i, row_temp, col_temp);
                    }
                }
            }

        }

    }
}



int main() {
    srand(static_cast<unsigned>(time(0))); // Seed the random number generator
    vector<SudokuBoard> boards = initializeBoards();
    map<string, sf::Texture> textures = initializeTextures();
    AlgorithmManager Manager;
    sf::RenderWindow welcome_window(sf::VideoMode(1280, 720), "Speedoku");
    welcome_window.clear(sf::Color::White);

    sf::Font font;
    font.loadFromFile("../../assets/arial.ttf");

    sf::Sprite sudoku_icon;
    sudoku_icon.setTexture(textures["sudoku_icon"]);
    sudoku_icon.setOrigin(sf::Vector2f(textures["sudoku_icon"].getSize().x / 2, textures["sudoku_icon"].getSize().y / 2));
    sudoku_icon.setScale(0.75f, 0.75f);
    sudoku_icon.setPosition(640, 350);

    sf::Sprite speedoku_text;
    speedoku_text.setTexture(textures["Speedoku"]);
    speedoku_text.setOrigin(sf::Vector2f(textures["Speedoku"].getSize().x / 2, textures["Speedoku"].getSize().y / 2));
    speedoku_text.setScale(0.5f, 0.5f);
    speedoku_text.setPosition(640, 80);

    sf::Sprite start_icon;
    start_icon.setTexture(textures["start_icon"]);
    start_icon.setOrigin(sf::Vector2f(textures["start_icon"].getSize().x / 2, textures["start_icon"].getSize().y / 2));
    start_icon.setScale(0.65f, 0.65f);
    start_icon.setPosition(640, 630);


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
                if (mode == 1) {
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                        mode = 2;
                    }
                }
                else if (mode == 2){
                    cout<<"im in";
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                        welcome_window.clear(sf::Color::White);
                        mode = 1;
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
            auto start = chrono::high_resolution_clock::now();
            solveBacktracking(boardAlgo1, welcome_window, false);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = (end - start);
            //update manager with Backtracking run
            Manager.updateAlgo("backtracking", elapsed.count());
            sf::sleep(sf::milliseconds(2500));

            drawSudokuBoard(boards[board], welcome_window, 1);
            sf::sleep(sf::milliseconds(2500));
            SudokuBoard boardAlgo2 = boards[board];
            start = chrono::high_resolution_clock::now();
            solveCrossHatch(boardAlgo2, welcome_window);
            end = std::chrono::high_resolution_clock::now();
            elapsed = (end - start);
            Manager.updateAlgo("crosshatching", elapsed.count());
            sf::sleep(sf::seconds(2));

            drawSudokuBoard(boards[board], welcome_window, 2);
            sf::sleep(sf::milliseconds(2500));
            SudokuBoard boardAlgo3 = boards[board];
            start = chrono::high_resolution_clock::now();
            solveNakedSingle(boardAlgo3, welcome_window);
            end = std::chrono::high_resolution_clock::now();
            elapsed = (end - start);
            Manager.updateAlgo("NakedSingle", elapsed.count());
            sf::sleep(sf::milliseconds(2500));

            drawSudokuBoard(boards[board], welcome_window, 3);
            sf::sleep(sf::milliseconds(2500));
            SudokuBoard boardAlgo4 = boards[board];
            start = chrono::high_resolution_clock::now();
            solveHiddenSingles(boardAlgo4);
            drawSudokuBoard(boards[board], welcome_window, 3);
            solveBacktracking(boardAlgo4, welcome_window, true);
            end = std::chrono::high_resolution_clock::now();
            elapsed = (end - start);
            Manager.updateAlgo("HiddenSingle", elapsed.count());
            sf::sleep(sf::milliseconds(2500));

            board++;


        } else if(mode == 2) {
            // TODO: Implement post-solve time comparison screen.
            // TODO: Implement post-solve time comparison screen.
            welcome_window.clear(sf::Color::White);
            float screenWidth = welcome_window.getSize().x;

            sf::Text enter_Text;
            enter_Text.setFont(font);
            enter_Text.setCharacterSize(48);
            enter_Text.setFillColor(sf::Color::Black);
            enter_Text.setString("Press Enter To Go Back To Algorithms!");

            sf::FloatRect textBounds6 = enter_Text.getLocalBounds();
            enter_Text.setPosition(640 - textBounds6.width/2, 10); // Adjust the position based on your layout


            //backtracking text object
            sf::Text bt_text;
            bt_text.setFont(font);
            bt_text.setCharacterSize(36); //font size
            bt_text.setFillColor(sf::Color::Black);
            string bt_average = to_string(Manager.getAverage("backtracking"));
            bt_average = bt_average.substr(0, 4);
            bt_text.setString("Backtracking average time: " + bt_average+ " seconds");
            sf::FloatRect textBounds = bt_text.getLocalBounds();
            float textWidth = textBounds.width;
            bt_text.setPosition((screenWidth - textWidth) / 2, 300);

            //crosshatching text object
            sf::Text ch_text;
            ch_text.setFont(font);
            ch_text.setCharacterSize(36); //font size
            ch_text.setFillColor(sf::Color::Black);
            string ch_average = to_string(Manager.getAverage("crosshatching"));
            ch_average = ch_average.substr(0, 4);
            ch_text.setString("Crosshatching average time: " + ch_average+ " seconds" );
            sf::FloatRect textBounds2 = ch_text.getLocalBounds();
            float textWidth2 = textBounds2.width;
            ch_text.setPosition((screenWidth - textWidth2) / 2, 350);

            sf::Text nt_text;
            nt_text.setFont(font);
            nt_text.setCharacterSize(36); //font size
            nt_text.setFillColor(sf::Color::Black);
            string nt_average = to_string(Manager.getAverage("NakedSingle"));
            nt_average = nt_average.substr(0, 4);
            nt_text.setString("Naked Single average time: " + nt_average + " seconds");
            sf::FloatRect textBounds3 = nt_text.getLocalBounds();
            float textWidth3 = textBounds3.width;
            nt_text.setPosition((screenWidth - textWidth3) / 2, 400);

            sf::Text hs_text;
            hs_text.setFont(font);
            hs_text.setCharacterSize(36); //font size
            hs_text.setFillColor(sf::Color::Black);
            string hs_average = to_string(Manager.getAverage("HiddenSingle"));
            hs_average = hs_average.substr(0, 4);
            hs_text.setString("Hidden Single average time: " + hs_average + " seconds");
            sf::FloatRect textBounds5 = hs_text.getLocalBounds();
            float textWidth5 = textBounds5.width;
            hs_text.setPosition((screenWidth - textWidth5) / 2, 450);

            sf::Sprite comparisons;
            comparisons.setTexture(textures["comparisons"]);
            comparisons.setScale(0.5, 0.5f);
            sf::FloatRect textBounds4 = comparisons.getLocalBounds();
            float textWidth4 = textBounds4.width;
            comparisons.setPosition((screenWidth - textWidth4) + 325, 120);

            welcome_window.draw(comparisons);
            welcome_window.draw(bt_text);
            welcome_window.draw(ch_text);
            welcome_window.draw(nt_text);
            welcome_window.draw(hs_text);
            welcome_window.draw(enter_Text);
            welcome_window.display();
        }
    }

    return 0;


}