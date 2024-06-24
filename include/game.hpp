#pragma once
#include <array>

constexpr int cellHeight = 2;
constexpr int cellWidth = 4;
constexpr int unset = -1;
constexpr int defaultColorPair = 1;
constexpr int cursorColorPair = 2;
constexpr int offsetY = 1;
constexpr int offsetX = 1;

class Game {
public:
    Game();
    ~Game();
    void start();
    void update_loop();
    int at(int row, int col) const;
    void set(int row, int col, int value);
    void set_at_cursor(int value);
    int at_cursor() const;
    void translate_cursor(int dRow, int dCol);
    void move_cursor(int row, int col);

private:
    int cursorY = 0, cursorX = 0;
    std::array<std::array<int, 9>, 9> game{};
    std::array<std::array<int, 9>, 9> solution{};
    void draw_cell(int row, int col, int value) const;
    void init_view() const;
    void draw_grid() const;
    void refresh_view() const;
    void draw_help() const;
};