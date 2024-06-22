#include <game.hpp>
#include <ncurses.h>

Game::Game() {
    // init game board
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            game[row][col] = col + 1;
        }
    }
}

Game::~Game() {
    endwin();
}

void Game::init_view() {
    initscr();
    mvaddstr(0, cellWidth, "Sudokurse -- Easy");
    draw_grid(1, 1);
    curs_set(0);
    getch();
}

// a value = -1 is equivalent to empty
void Game::draw_cell(int row, int col, int value) {
    if (value < 1 || value > 9) {
        value = -1;
    }
    mvaddstr(row * cellHeight, col * cellWidth,     "+---+");
    mvaddstr(row * cellHeight + 1, col * cellWidth, "|   |");
    mvaddstr(row * cellHeight + 2, col * cellWidth, "+---+");
    if (value != -1) {
        mvaddch(row * cellHeight + 1, col * cellWidth + 2, value + '0');
    }
}

void Game::draw_grid(int offsetY, int offsetX) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            draw_cell(row + offsetY, col + offsetX, game[row][col]);
        }
    }
}