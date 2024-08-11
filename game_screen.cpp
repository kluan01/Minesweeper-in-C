#include <SFML/Graphics.hpp>
#include <vector>
#include "functions.h"
using namespace std;
using namespace sf;

void game_screen() {
    // Load all sprites from files
    Texture debug_text; debug_text.loadFromFile("files/images/debug.png"); Sprite debug(debug_text);
    Texture digits_text; digits_text.loadFromFile("files/images/digits.png"); Sprite digits(digits_text);
    Texture face_happy_text; face_happy_text.loadFromFile("files/images/face_happy.png");Sprite face_happy(face_happy_text);
    Texture face_lose_text; face_lose_text.loadFromFile("files/images/face_lose.png"); Sprite face_lose(face_lose_text);
    Texture face_win_text; face_win_text.loadFromFile("files/images/face_win.png"); Sprite face_win(face_win_text);
    Texture flag_text; flag_text.loadFromFile("files/images/flag.png"); Sprite flag(flag_text);
    Texture mine_text; mine_text.loadFromFile("files/images/mine.png"); Sprite mine(mine_text);
    Texture number_1_text; number_1_text.loadFromFile("files/images/number_1.png"); Sprite number_1(number_1_text);
    Texture number_2_text; number_2_text.loadFromFile("files/images/number_2.png"); Sprite number_2(number_2_text);
    Texture number_3_text; number_3_text.loadFromFile("files/images/number_3.png"); Sprite number_3(number_3_text);
    Texture number_4_text; number_4_text.loadFromFile("files/images/number_4.png"); Sprite number_4(number_4_text);
    Texture number_5_text; number_5_text.loadFromFile("files/images/number_5.png"); Sprite number_5(number_5_text);
    Texture number_6_text; number_6_text.loadFromFile("files/images/number_6.png"); Sprite number_6(number_1_text);
    Texture number_7_text; number_7_text.loadFromFile("files/images/number_7.png"); Sprite number_7(number_7_text);
    Texture number_8_text; number_8_text.loadFromFile("files/images/number_8.png"); Sprite number_8(number_8_text);
    Texture pause_text; pause_text.loadFromFile("files/images/pause.png"); Sprite pause(pause_text);
    Texture play_text; play_text.loadFromFile("files/images/play.png"); Sprite play(play_text);
    Texture tile_hidden_text; tile_hidden_text.loadFromFile("files/images/tile_hidden.png"); Sprite tile_hidden(tile_hidden_text);
    Texture tile_revealed_text; tile_revealed_text.loadFromFile("files/images/tile_revealed.png"); Sprite tile_revealed(tile_revealed_text);

    // Load Digit sprites for clock
    vector<Sprite> sprite_digits;
    const int digit_width = 21;
    const int digit_height = 32;
    for(int i = 0; i < 11; i++) {
        IntRect textureRect(i * digit_width, 0, digit_width, digit_height);
        Sprite digit_sprite(digits_text);
        digit_sprite.setTextureRect(textureRect);
        sprite_digits.push_back(digit_sprite);
    }

    Sprite first_min_sprite; // Minutes
    Sprite second_min_sprite;
    Sprite first_sec_sprite; // Seconds
    Sprite second_sec_sprite;

    // Declare game attributes with config data
    int screen_width = (config_data[0] * 32);
    int screen_height = ((config_data[1] * 32) + 100);
    int mine_count = config_data[2];
    int num_col = config_data[0];
    int num_row = config_data[1];
    RenderWindow window(VideoMode(screen_width, screen_height), "Minesweeper");

    // Setup NAVBAR sprites
    face_happy.setPosition((((num_col/2.0) * 32) - 32), (32 * (num_row + 0.5)));
    debug.setPosition(((num_col * 32) - 304),(32 * (num_row + 0.5)));
    pause.setPosition(((num_col * 32) - 240),(32 * (num_row + 0.5)));

    // Declare board and game variables
    vector<vector<Tile>> awesome_board = create_board(num_col, num_row, mine_count);
    bool is_paused = false;
    bool is_debugged = false;
    bool game_won = false;
    bool game_lost = false;

    auto start = chrono::high_resolution_clock::now(); // Time Variables
    auto pause_start = chrono::high_resolution_clock::now();
    auto pause_end = chrono::high_resolution_clock::now();
    auto pause_time = chrono::duration_cast<chrono::seconds>(pause_end - pause_start);

    // Game loop
    while (window.isOpen()) {

        // Clock Feature - Calculating pause time
        auto end = chrono::high_resolution_clock::now();
        auto duration_mins = chrono::duration_cast<chrono::minutes>(end - start);
        auto duration_secs = chrono::duration_cast<chrono::seconds>(end - start) - pause_time;

        int mins_passed = (static_cast<int>(duration_mins.count())) % 60; // chrono -> int -> sprite digits
        int secs_passed = (static_cast<int>(duration_secs.count())) % 60;

        int first_digit_min = static_cast<int>(mins_passed / 10) % 10; // Minutes display
        int second_digit_min = mins_passed % 10;

        int first_digit_sec = static_cast<int>(secs_passed / 10) % 10; // Seconds display
        int second_digit_sec = secs_passed % 10;

        // Change clock digits to reflect when time was paused
        if(!is_paused) {
            first_min_sprite = sprite_digits[first_digit_min];
            second_min_sprite = sprite_digits[second_digit_min];
            first_sec_sprite = sprite_digits[first_digit_sec];
            second_sec_sprite = sprite_digits[second_digit_sec];
        }

        // Set clock digits
        first_min_sprite.setPosition((num_col * 32) - 97, (32 * (num_row + 0.5) + 16));
        second_min_sprite.setPosition((num_col * 32) - 97 + 21, (32 * (num_row + 0.5) + 16));
        first_sec_sprite.setPosition((num_col * 32) - 54, (32 * (num_row + 0.5) + 16));
        second_sec_sprite.setPosition((num_col * 32) - 54 + 21, (32 * (num_row + 0.5) + 16));

        // Declare boundaries for event detecting
        Vector2i cursor_pos_initial = Mouse::getPosition(window);
        Vector2f cursor_pos(cursor_pos_initial.x, cursor_pos_initial.y);

        FloatRect pause_bounds = pause.getGlobalBounds();
        FloatRect happy_bounds = face_happy.getGlobalBounds();
        FloatRect game_bounds(0, 0, num_col * 32, num_row * 32);
        FloatRect debug_bounds = debug.getGlobalBounds();

        // User input loop
        Event event;
        while (window.pollEvent(event)) {
            // User exits game
            if (event.type == Event::Closed) {
                window.close();
            }
                // User resets game
            else if (event.type == Event::MouseButtonPressed && happy_bounds.contains(cursor_pos)) {
                awesome_board = create_board(num_col, num_row, mine_count); // Create new board
                for (int curr_col = 0; curr_col < num_col; curr_col++) {
                    for (int curr_row = 0; curr_row < num_row; curr_row++) {
                        awesome_board[curr_col][curr_row].is_hidden = true;
                    }
                }
                start = chrono::high_resolution_clock::now(); // Update variables for game features
                face_happy.setTexture(face_happy_text);
                pause.setTexture(pause_text);
                is_paused = false;
                game_won = false;
                game_lost = false;
                mine_count = config_data[2];
            }

            // User pauses the game
            else if (event.type == Event::MouseButtonPressed && pause_bounds.contains(cursor_pos) && !game_won &&
                     !game_lost) {
                is_paused = !is_paused;
                if (is_paused) {
                    pause.setTexture(play_text);
                    pause_start = chrono::high_resolution_clock::now();
                } else {
                    pause.setTexture(pause_text);
                    pause_end = chrono::high_resolution_clock::now();
                    pause_time += chrono::duration_cast<chrono::seconds>(pause_end - pause_start);

                }
            }

            // User clicks on a tile
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left &&
                     game_bounds.contains(cursor_pos) && !is_paused) {
                int hovered_col = cursor_pos_initial.x / 32; // Find position on board
                int hovered_row = cursor_pos_initial.y / 32;
                if (hovered_col >= 0 && hovered_col < num_col && hovered_row >= 0 && hovered_row < num_row &&
                    !awesome_board[hovered_col][hovered_row].is_flagged) {
                    awesome_board[hovered_col][hovered_row].is_hidden = false;

                    // User clicks on a mine!!
                    if (awesome_board[hovered_col][hovered_row].is_mine) {
                        for (int curr_col = 0; curr_col < num_col; curr_col++) {
                            for (int curr_row = 0; curr_row < num_row; curr_row++) {
                                if (awesome_board[curr_col][curr_row].is_mine) {
                                    awesome_board[curr_col][curr_row].is_hidden = false;
                                }
                            }
                        }
                        face_happy.setTexture(face_lose_text);
                        is_paused = true;
                        game_lost = true;
                    }
                }
            }

            // User flags a tile
            else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Right &&
                     game_bounds.contains(cursor_pos) && !is_paused) {
                int hovered_col = cursor_pos_initial.x / 32; // Find position on board
                int hovered_row = cursor_pos_initial.y / 32;
                if (hovered_col >= 0 && hovered_col < num_col && hovered_row >= 0 && hovered_row < num_row) {
                    if (awesome_board[hovered_col][hovered_row].is_hidden) {
                        awesome_board[hovered_col][hovered_row].is_flagged = !awesome_board[hovered_col][hovered_row].is_flagged;
                        if (awesome_board[hovered_col][hovered_row].is_flagged) { // Update mine count
                            mine_count -= 1;
                        }
                        if (!awesome_board[hovered_col][hovered_row].is_flagged) {
                            mine_count += 1;
                        }
                    }
                }
            }

            // User clicks debug button
            else if (event.type == Event::MouseButtonPressed && debug_bounds.contains(cursor_pos) && !is_paused) {
                is_debugged = !is_debugged;
                if (is_debugged) { // Reveal tiles
                    for (int curr_col = 0; curr_col < num_col; curr_col++) {
                        for (int curr_row = 0; curr_row < num_row; curr_row++) {
                            if (awesome_board[curr_col][curr_row].is_mine) {
                                awesome_board[curr_col][curr_row].is_hidden = true;
                            }
                        }
                    }
                } else { // Hide tiles
                    for (int curr_col = 0; curr_col < num_col; curr_col++) {
                        for (int curr_row = 0; curr_row < num_row; curr_row++) {
                            if (awesome_board[curr_col][curr_row].is_mine) {
                                awesome_board[curr_col][curr_row].is_hidden = false;
                            }
                        }
                    }
                }
            }
        }

        // User obtains negative mine count
        Sprite neg_sprite;
        if(mine_count < 0) {
            neg_sprite = sprite_digits[10];
            neg_sprite.setPosition(12, (32* (num_row + 0.5) + 16));
        }

        int first_digit_mine = static_cast<int>(mine_count / 100) % 10;
        int second_digit_mine = static_cast<int>(mine_count / 10) % 10;
        int third_digit_mine = mine_count % 10;
        if (third_digit_mine < 0) {
            third_digit_mine *= -1;
        }
        if (second_digit_mine < 0) {
            second_digit_mine *= -1;
        }

        // Declare and set negative mine count
        Sprite first_digit_sprite = sprite_digits[first_digit_mine];
        Sprite second_digit_sprite = sprite_digits[second_digit_mine];
        Sprite third_digit_sprite = sprite_digits[third_digit_mine];
        first_digit_sprite.setPosition(33, (32* (num_row + 0.5) + 16));
        second_digit_sprite.setPosition(33 + 21, (32* (num_row + 0.5) + 16));
        third_digit_sprite.setPosition(33 + 21 + 21, (32* (num_row + 0.5) + 16));

        // Draw game sprites
        window.clear(Color::White);
        window.draw(face_happy);
        window.draw(debug);
        window.draw(pause);
        window.draw(first_digit_sprite);
        window.draw(second_digit_sprite);
        window.draw(third_digit_sprite);
        window.draw(neg_sprite);
        window.draw(first_sec_sprite);
        window.draw(second_sec_sprite);
        window.draw(first_min_sprite);
        window.draw(second_min_sprite);

        // Drawing tiles on board (and tile features)
        for(int curr_col = 0; curr_col < num_col; curr_col++) {
            for(int curr_row = 0; curr_row < num_row; curr_row++) {
                // Hidden tile drawing
                if(awesome_board[curr_col][curr_row].is_hidden) {
                    tile_hidden.setPosition(curr_col * 32, curr_row * 32);
                    window.draw(tile_hidden);
                    if(awesome_board[curr_col][curr_row].is_flagged) { // Flag drawing
                        flag.setPosition(curr_col * 32, curr_row * 32);
                        window.draw(flag);
                    }
                }

                    // Revealed tile drawing
                else if(!awesome_board[curr_col][curr_row].is_hidden) {
                    tile_revealed.setPosition(curr_col * 32, curr_row * 32);
                    window.draw(tile_revealed);

                    // Tile mine count
                    if(awesome_board[curr_col][curr_row].neighbors == 0 && !awesome_board[curr_col][curr_row].is_mine) {
                        for (int i = -1; i <= 1; i++) {
                            for (int j = -1; j <= 1; j++) {
                                int neighbor_col = curr_col + i;
                                int neighbor_row = curr_row + j;

                                // Remove all tiles surrounding user input tile if there are no neighboring mines
                                if (neighbor_col >= 0 && neighbor_col < num_col && neighbor_row >= 0 &&
                                    neighbor_row < num_row && !awesome_board[neighbor_col][neighbor_row].is_flagged) {
                                    awesome_board[neighbor_col][neighbor_row].is_hidden = false;
                                }
                            }
                        }
                    }
                    if(awesome_board[curr_col][curr_row].neighbors == 1) {
                        number_1.setPosition(curr_col * 32, curr_row * 32);
                        window.draw(number_1);
                    }
                    if(awesome_board[curr_col][curr_row].neighbors == 2) {
                        number_2.setPosition(curr_col * 32, curr_row * 32);
                        window.draw(number_2);
                    }
                    if(awesome_board[curr_col][curr_row].neighbors == 3) {
                        number_3.setPosition(curr_col * 32, curr_row * 32);
                        window.draw(number_3);
                    }
                    if(awesome_board[curr_col][curr_row].neighbors == 4) {
                        number_4.setPosition(curr_col * 32, curr_row * 32);
                        window.draw(number_4);
                    }
                    if(awesome_board[curr_col][curr_row].neighbors == 5) {
                        number_5.setPosition(curr_col * 32, curr_row * 32);
                        window.draw(number_5);
                    }
                    if(awesome_board[curr_col][curr_row].neighbors == 6) {
                        number_6.setPosition(curr_col * 32, curr_row * 32);
                        window.draw(number_6);
                    }
                    if(awesome_board[curr_col][curr_row].neighbors == 7) {
                        number_7.setPosition(curr_col * 32, curr_row * 32);
                        window.draw(number_7);
                    }
                    if(awesome_board[curr_col][curr_row].neighbors == 8) {
                        number_8.setPosition(curr_col * 32, curr_row * 32);
                        window.draw(number_8);
                    }
                    if(awesome_board[curr_col][curr_row].is_mine) {
                        mine.setPosition(curr_col * 32, curr_row * 32);
                        window.draw(mine);
                    }
                }
            }
        }
        window.display(); // Update window

        // Win Authentication
        int counting = 0;
        for(int q = 0; q < num_col; q++) {
            for(int w = 0; w < num_row; w++) {
                if (!awesome_board[q][w].is_mine) {
                    if (!awesome_board[q][w].is_hidden) {
                        counting += 1;
                    }
                }
            }
        }
        if(counting == (num_col * num_row) - config_data[2]) {
            face_happy.setTexture(face_win_text);
            game_won = true;
        }

        if(game_won) {
            for (int curr_col = 0; curr_col < num_col; curr_col++) {
                for (int curr_row = 0; curr_row < num_row; curr_row++) {
                    if (awesome_board[curr_col][curr_row].is_mine) {
                        awesome_board[curr_col][curr_row].is_flagged = true;
                    }
                }
            }
            is_paused = true;
        }
    }
}
