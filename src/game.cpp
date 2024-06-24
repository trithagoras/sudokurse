#include <game.hpp>
#include <ncurses.h>
#include <cstdlib>
#include "utils.hpp"
#include <iostream>
#include "sudoku.hpp"

std::string difficulty_str(Difficulty d) {
    switch (d) {
        case Difficulty::Easy:
            return "easy";
        case Difficulty::Medium:
            return "medium";
        case Difficulty::Hard:
            return "hard";
        case Difficulty::Master:
            return "master";
    }
    return "UNKNOWN";
}

Game::Game() {
    srand(time(NULL));  // init random seed

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

void Game::reset_game() {
    // initialize solution board and initial game state
    auto puzzle = Sudoku();
    puzzle.createSeed();
    puzzle.genPuzzle();
    puzzle.calculateDifficulty();
    if (puzzle.difficultyLevel <= 400) {
        difficulty = Difficulty::Easy;
    } else if (puzzle.difficultyLevel <= 800) {
        difficulty = Difficulty::Medium;
    } else if (puzzle.difficultyLevel <= 1100) {
        difficulty = Difficulty::Hard;
    } else {
        difficulty = Difficulty::Master;
    }

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            game[r][c] = puzzle.grid[r][c];
            solution[r][c] = puzzle.solnGrid[r][c];
            initialState[r][c] = puzzle.grid[r][c];
        }
    }
}

void Game::start() {
    init_view();
    reset_game();
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
    init_color(COLOR_GRAY, 372, 372, 372);
    init_pair(lightColorPair, COLOR_GRAY, COLOR_BLACK);
    init_pair(cursorColorPair, COLOR_BLACK, COLOR_WHITE);
    init_pair(yellowColorPair, COLOR_YELLOW, COLOR_BLACK);
    init_pair(greenColorPair, COLOR_GREEN, COLOR_BLACK);
    init_pair(redColorPair, COLOR_RED, COLOR_BLACK);
}

void Game::refresh_view() {
    auto titleStr = "Sudokurse -- " + difficulty_str(difficulty);
    mvaddstr(0, cellWidth * offsetX, titleStr.c_str());
    draw_grid();

    // draw cursor
    attron(COLOR_PAIR(cursorColorPair));
    mvaddch(cursorY * cellHeight + (offsetY * cellHeight) + 1, cursorX * cellWidth + (offsetX * cellWidth) + 2, int_to_char(at_cursor()));
    attroff(COLOR_PAIR(cursorColorPair));

    draw_help();

    // draw error / success
    if (errorText != "" || successText != "") {
        if (errorText != "") {
            attron(COLOR_PAIR(redColorPair));
            mvaddstr(23, 3, errorText.c_str());
            attroff(COLOR_PAIR(redColorPair));
        } else if (successText != "") {
            attron(COLOR_PAIR(greenColorPair));
            mvaddstr(23, 3, successText.c_str());
            attroff(COLOR_PAIR(greenColorPair));
        }
        errorText = "";
        successText = "";
    }
}

void Game::draw_help() const {
    int x = 47;
    int y = 3;
    mvaddstr(y++, x, "Help");
    mvaddstr(y++, x, "[1..9] Set number at cursor position");
    mvaddstr(y++, x, "[SPACE] Unset number at cursor position");
    mvaddstr(y++, x, "[ENTER] Present solution");
    y++;
    mvaddstr(y++, x, "[R] Reset this game");
    mvaddstr(y++, x, "[N] New game    [Q] Quit game");
    y++;
    mvaddstr(y++, x, "[S] Auto solve");
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
                set_at_cursor(unset);
                break;
            case 'n':
            case 'N':
                reset_game();
                break;
            case 'R':
            case 'r':
                game = initialState;
                break;
            case KEY_ENTER:
            case 10:    // \n
                try_solve();
                break;
            case 'S':
            case 's':
                auto_solve();
                break;
        }
        refresh();
    }
}

// a value = 0 is equivalent to empty
void Game::draw_cell(int row, int col, int value) const {
    if (value < 1 || value > 9) {
        value = unset;
    }
    int y = row + offsetY;
    int x = col + offsetX;

    attron(COLOR_PAIR(lightColorPair));
    mvaddstr(y * cellHeight, x * cellWidth,     "+---+");
    mvaddstr(y * cellHeight + 1, x * cellWidth, "|   |");
    mvaddstr(y * cellHeight + 2, x * cellWidth, "+---+");
    attroff(COLOR_PAIR(lightColorPair));

    if (value != unset) {
        // if value is in initialState, draw in different color
        if (initialState[row][col] != unset) {
            mvaddch(y * cellHeight + 1, x * cellWidth + 2, int_to_char(value));
        } else {
            attron(COLOR_PAIR(yellowColorPair));
            mvaddch(y * cellHeight + 1, x * cellWidth + 2, int_to_char(value));
            attroff(COLOR_PAIR(yellowColorPair));
        }
    }
}

void Game::draw_grid() const {
    // draw all grid lines in gray color
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            draw_cell(row, col, game[row][col]);
        }
    }

    // then main segments in white
    for (int row = 0; row <= 3; row++) {
        mvaddstr((offsetY * cellHeight) + (row * cellHeight * 3), (offsetX * cellWidth), "+---+---+---+---+---+---+---+---+---+");
    }

    for (int col = 0; col <= 3; col++) {
        for (int row = 0; row < 9; row++) {
            mvaddch((offsetY * cellHeight) + (row * cellHeight), (offsetX * cellWidth) + (col * cellWidth * 3), '+');
            mvaddch((offsetY * cellHeight) + (row * cellHeight) + 1, (offsetX * cellWidth) + (col * cellWidth * 3), '|');
        }
    }
}

int Game::at(int row, int col) const {
    return game[row][col];
}
void Game::set(int row, int col, int value) {
    // do nothing if this is initial value
    if (initialState[row][col] != unset) {
        return;
    }
    if (value < 1 || value > 9) {
        value = unset;
    }
    game[row][col] = value;
}
int Game::at_cursor() const {
    return game[cursorY][cursorX];
}
void Game::translate_cursor(int dRow, int dCol) {
    move_cursor(cursorY + dRow, cursorX + dCol);
}
void Game::move_cursor(int row, int col) {
    // prevent OOB
    if (row < 0 || row >= 9 || col < 0 || col >= 9) {
        return;
    }
    cursorY = row;
    cursorX = col;
}
void Game::set_at_cursor(int value) {
    set(cursorY, cursorX, value);
}

void Game::try_solve() {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (game[row][col] != solution[row][col]) {
                errorText = "Your solution is incorrect.";
            }
        }
    }
    successText = "Congratulations! You solved the puzzle!";
}

void Game::auto_solve() {
    game = solution;
}