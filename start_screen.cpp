#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <sstream>
#include "functions.h"
#include <cstdlib>
using namespace std;
using namespace sf;

// Declaration for global use
vector<int> config_data;

// SFML given function to align obj/text
void setText(sf::Text &text, float x, float y){ sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}
// Starting Screen
bool run_start_screen() {

    // Open file for starting information
    ifstream config_file("files/config.cfg");
    if (!config_file.is_open()) {
        cout << "Issue w/ Config File" << endl;
    }

    // Initialize starting information into tangible data (str -> int)
    string config_line;
    while (getline(config_file, config_line)) {
        istringstream iss(config_line);
        string config_number;
        while (iss >> config_number) {
            config_data.push_back(stoi(config_line));
        }
    }
    config_file.close();

    // Declare starting screen size
    int screen_width = (config_data[0] * 32);
    int screen_height = ((config_data[1] * 32) + 100);
    RenderWindow window(VideoMode(screen_width, screen_height), "Minesweeper");
    string name;

    // Declare and set starting titles and headers
    Font font;
    font.loadFromFile("files/font.ttf");
    Text text_1("WELCOME TO MINESWEEPER!", font, 24);
    text_1.setStyle(Text::Bold | Text::Underlined);
    setText(text_1,(screen_width/2), ((screen_height/2) - 150));

    Text text_2("Enter your name:", font, 20);
    text_2.setStyle(Text::Bold);
    setText(text_2, (screen_width/2), ((screen_height/2) - 75));

    Text cursor("|", font, 18);
    cursor.setStyle(Text::Bold);
    cursor.setFillColor(Color::Yellow);
    setText(cursor,(screen_width/2), ((screen_height/2) - 45));

    Text user_input("", font, 18);
    user_input.setStyle(Text::Bold);
    user_input.setFillColor(Color::Yellow);
    setText(user_input,(screen_width/2), ((screen_height/2) - 57.5));

    // Starting screen loop for name authentication
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) { // Closing Screen
                window.close();
                return false;
            } else if (event.type == Event::KeyPressed) { // Keyboard -> SFML value
                Keyboard::Key current_key = event.key.code;

                if(current_key == Keyboard::BackSpace && !name.empty()) { // Backspace
                    name.pop_back();
                }
                else if (current_key >= Keyboard::A && current_key <= Keyboard::Z) { // Input Character
                    if(name.size() < 10) {
                        char letter = 'A' + (current_key - Keyboard::A);
                        if(name.empty()) {
                            letter = toupper(letter); // Uppercase first letter
                        }
                        else {
                            letter = tolower(letter); // Lowercase for all other characters
                        }
                        name += letter;
                    }
                }
                else if(current_key == Keyboard::Enter && name.size() >= 1) { // Name validation success
                    window.close();
                    return true;
                }
            }
        }

        // Dynamically update user's name and cursor
        user_input.setString(name);
        int name_length = static_cast<int>(name.size());
        user_input.setPosition((screen_width/2) - (name_length * 6), ((screen_height/2) - 57.5));
        cursor.setPosition(user_input.findCharacterPos(name.size()).x, cursor.getPosition().y);

        // Draw all starting window texts/sprites
        window.clear(Color::Blue);
        window.draw(text_1);
        window.draw(text_2);
        window.draw(user_input);
        window.draw(cursor);
        window.display();
    }
}
