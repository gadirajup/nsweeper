// Copyright 2020

#include <stdlib.h>
#include <ncurses.h>
#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>
#include "minesweeper.hpp"


void update_display(WINDOW *win, Minesweeper game) {
    wmove(win, 0, 1);
    waddstr(win, "nsweeper");

    char character_map[9] = {'_', '1', '2', '3', '4', '5', '6', '7', '8'};
    int current_row = 1;
    for (int i = 0; i < game.height; i++) {
        wmove(win, current_row, 1);
        for (int j = 0; j < game.width; j++) {
            char character = ' ';

            Node *cell = &game.board[i * game.width + j];
            if (cell->visited) {
                character = character_map[cell->value];
            }

            if (cell->is_mine && !cell->is_flag && game.hit_mine) {
                character = '*';
            } else if (cell->is_mine && game.game_finished && !game.hit_mine) {
                character = 'F';
            }

            if (cell->is_flag) {
                character = 'F';
            }

            waddch(win, character);
        }
        current_row++;
    }
}

void displayIntroMessage() {
    std::cout << "WELCOME TO NSWEEPER" << std::endl;
}

int getDifficultySelection() {
    int selection;

    std::cout << "Choose level of difficulty:\n\n";
    std::cout << "1. Easy 8 x 8 board (10 mines)\n";
    std::cout << "2. Intermediate 16 x 16 board (40 mines)\n";
    std::cout << "3. Advanced 16 x 30 board (99 mines)\n\n";

    std::cin >> selection;
    std::cout << selection << std::endl;
    return selection;
}

int main() {
    const int KEY_ENTE = 10;
    const int KEY_F = 102;
    const int KEY_R = 114;

    int input;
    bool restart = true;
    int cur_x;
    int cur_y;

    int width = 8;
    int height = 8;
    int mines = 10;

    int win_start_x = 0;
    int win_start_y = 0;

    displayIntroMessage();

    switch (static_cast<int>(getDifficultySelection())) {
        case 1:
            height = 8;
            width = 8;
            mines = 10;
            break;
        case 2:
            height = 16;
            width = 16;
            mines = 40;
            break;
        case 3:
            height = 16;
            width = 30;
            mines = 99;
            break;
        default:
            std::cout << "Invalid Option. Autoselecting option 2" << std::endl;
            height = 16;
            width = 16;
            mines = 40;
            break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    Minesweeper game = Minesweeper(height, width, mines);

    initscr();
    noecho();
    clear();
    nodelay(stdscr, true);
    keypad(stdscr, true);

    WINDOW *win = newwin(height + 2, width + 2, win_start_y, win_start_x);

    while (restart) {
        cur_x = 1;
        cur_y = 1;

        refresh();
        box(win, 0, 0);
        update_display(win, game);
        wmove(win, 1, 1);
        wrefresh(win);
        restart = false;

        while (!game.game_finished) {
            if ((input = getch()) != ERR) {
                switch (input) {
                    case KEY_DOWN:
                        if (cur_y < height) { cur_y++; }
                        break;
                    case KEY_UP:
                        if (cur_y > 1) { cur_y--; }
                        break;
                    case KEY_RIGHT:
                        if (cur_x < width) { cur_x++; }
                        break;
                    case KEY_LEFT:
                        if (cur_x > 1) { cur_x--; }
                        break;
                    case KEY_ENTE:
                        game.open_cell(cur_y - 1, cur_x - 1);
                        break;
                    case KEY_F:
                        game.toggle_flag(cur_y - 1, cur_x - 1);
                        break;
                    default:
                        break;
                }

                update_display(win, game);
                wmove(win, cur_y, cur_x);
                wrefresh(win);
            }
        }

        move(height + 3, 0);

        if (game.hit_mine) {
            printw("Hit Mine!\n");
        } else {
            printw("You win!\n");
            printw("You did it in %.2f seconds.\n", game.get_playing_time());
        }

        printw("Press 'R' to restart, 'ENTER' to quit...");
        refresh();

        while (1) {
            input = getch();
            if (input != ERR) {
                if (input == KEY_R) {
                    game.restart();
                    restart = true;
                    clear();
                    break;
                } else if (input == 10) {
                    break;
                }
            }
        }
    }

    delwin(win);
    endwin();

    return 0;
}
