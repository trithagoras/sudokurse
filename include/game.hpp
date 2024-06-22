#pragma once
#include <array>

constexpr int cellHeight = 2;
constexpr int cellWidth = 4;

class Game {
public:
    Game();
    ~Game();
    void init_view();
    void draw_grid(int offsetY, int offsetX);

private:
    std::array<std::array<int, 9>, 9> game{};
    void draw_cell(int row, int col, int value);
};