#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
using namespace std;
using namespace sf;

#ifndef MINDSWEEPER_FUNCTIONS_H
#define MINDSWEEPER_FUNCTIONS_H

// Tile class for game attributes
struct Tile {
    bool is_mine;
    bool is_hidden;
    bool is_flagged;
    int neighbors;
    Tile() : is_mine(false), is_hidden(true), is_flagged(false), neighbors(0) {}
};

// Header functions
void setText(sf::Text &text, float x, float y);
bool run_start_screen();
void game_screen();
vector<vector<Tile>> create_board(int num_cols, int num_rows, int mine_count);

// Global variables
extern vector<int> config_data;

#endif //MINDSWEEPER_FUNCTIONS_H
