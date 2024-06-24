#include <game.hpp>
#include <ncurses.h>
#include <cstdlib>
#include "utils.hpp"
#include <iostream>
#include <format>

Game::Game() {
    // init game board
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            game[row][col] = unset;
        }
    }
}

Game::~Game() {
    endwin();
}

void Game::start() {
    init_view();

    // initialize solution board and initial game state
    

    update_loop();
}

void Game::init_view() const {
    initscr();
    curs_set(0);
    keypad(stdscr, true);

    // init colors
    if (!has_colors()) {
        endwin();
        std::cerr << "Your terminal does not support color" << std::endl;
        std::exit(1);
    }

    start_color();
    init_pair(cursorColorPair, COLOR_BLACK, COLOR_WHITE);
}

void Game::refresh_view() const {
    mvaddstr(0, cellWidth * offsetX, "Sudokurse -- Easy");
    draw_grid();

    // draw cursor
    attron(COLOR_PAIR(cursorColorPair));
    mvaddch(cursorY * cellHeight + (offsetY * cellHeight) + 1, cursorX * cellWidth + (offsetX * cellWidth) + 2, int_to_char(at_cursor()));
    attroff(COLOR_PAIR(cursorColorPair));

    draw_help();
}

void Game::draw_help() const {
    int x = cellWidth * offsetX;
    mvaddstr(23, x, "Help");
    mvaddstr(24, x, "[1..9] Set number at cursor position");
    mvaddstr(25, x, "[SPACE] Unset number at cursor position");
    mvaddstr(26, x, "[Q] Quit game");
}

void Game::update_loop() {
    while (true) {
        erase();
        refresh_view();
        
        int ch = getch();
        switch (ch) {
            case 'q':
            case 'Q':
                return;
            case KEY_LEFT:
                translate_cursor(0, -1);
                break;
            case KEY_RIGHT:
                translate_cursor(0, 1);
                break;
            case KEY_UP:
                translate_cursor(-1, 0);
                break;
            case KEY_DOWN:
                translate_cursor(1, 0);
                break;
            case '1' ... '9':
                set_at_cursor(char_to_int(ch));
                break;
            case KEY_BACKSPACE:
            case ' ':
            case KEY_ENTER:
                set_at_cursor(unset);
                break;
        }
        refresh();
    }
}

// a value = -1 is equivalent to empty
void Game::draw_cell(int row, int col, int value) const {
    if (value < 1 || value > 9) {
        value = -1;
    }
    mvaddstr(row * cellHeight, col * cellWidth,     "+---+");
    mvaddstr(row * cellHeight + 1, col * cellWidth, "|   |");
    mvaddstr(row * cellHeight + 2, col * cellWidth, "+---+");
    if (value != -1) {
        mvaddch(row * cellHeight + 1, col * cellWidth + 2, int_to_char(value));
    }
}

void Game::draw_grid() const {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            draw_cell(row + offsetY, col + offsetX, game[row][col]);
        }
    }
}

int Game::at(int row, int col) const {
    return game[row][col];
}
void Game::set(int row, int col, int value) {
    if (value < 1 || value > 9) {
        value = unset;
    }
    game[row][col] = value;
}
int Game::at_cursor() const {
    return game[cursorY][cursorX];
}
void Game::translate_cursor(int dRow, int dCol) {
    cursorY += dRow;
    cursorX += dCol;
}
void Game::move_cursor(int row, int col) {
    cursorY = row;
    cursorX = col;
}
void Game::set_at_cursor(int value) {
    game[cursorY][cursorX] = value;
}