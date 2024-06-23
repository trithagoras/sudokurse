#pragma once
#include <array>

constexpr int cellHeight = 2;
constexpr int cellWidth = 4;
constexpr int unset = -1;

class Game {
public:
    Game();
    ~Game();
    void init_view();
    void draw_grid(int offsetY, int offsetX);
    int at(int row, int col);
    void set(int row, int col, int value);
    void set_at_cursor(int value);
    std::pair<int, int> at_cursor();
    void translate_cursor(int dRow, int dCol);
    void move_cursor(int row, int col);

private:
    int cursorY, cursorX;
    std::array<std::array<int, 9>, 9> game{};
    void draw_cell(int row, int col, int value);
};