#include "functions.h"
using namespace std;

// Main loop for Minesweeper
int main() {
    bool run_game = run_start_screen();
    if(run_game) {
        game_screen();
    }
    return 0;
}
