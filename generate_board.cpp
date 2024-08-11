#include <vector>
#include <cstdlib>
#include "functions.h"
using namespace std;
using namespace sf;

// Generates 2D Vector game board
vector<vector<Tile>> create_board(int num_cols, int num_rows, int mine_count) {

    // Initialize empty board
    vector<vector<Tile>> awesome_board(num_cols, vector<Tile>(num_rows));
    for(int col = 0; col < num_cols; col++) {
        for(int row = 0; row < num_rows; row++) {
            awesome_board[col][row] = Tile();
        }
    }

    // Randomly place mines on empty board
    for(int i = 0; i < mine_count; i++) {
        int rand_col = rand() % num_cols;
        int rand_row = rand() % num_rows;
        if(!awesome_board[rand_col][rand_row].is_mine) {
            awesome_board[rand_col][rand_row].is_mine = true;

            // Calculate neighboring mines
            for(int x_dir = -1; x_dir <= 1; x_dir++) {
                for(int y_dir = -1; y_dir <= 1; y_dir++) {
                    int current_col = rand_col + x_dir;
                    int current_row = rand_row + y_dir;
                    if(current_col >= 0 && current_col < num_cols && current_row >= 0 && current_row < num_rows
                       && !awesome_board[current_col][current_row].is_mine) {
                        awesome_board[current_col][current_row].neighbors += 1;
                    }
                }
            }
        }
        else {
            i--;
        }
    }
    return awesome_board;
}
