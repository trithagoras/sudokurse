#pragma once
#include <array>

constexpr int cellHeight = 2;
constexpr int cellWidth = 4;
constexpr int unset = 0;
constexpr int lightColorPair = 1;
constexpr int COLOR_GRAY = 8;
constexpr int cursorColorPair = 2;
constexpr int yellowColorPair = 3;
constexpr int offsetY = 1;
constexpr int offsetX = 1;

enum class Difficulty {
    Easy,
    Medium,
    Hard,
    Master
};

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
    Difficulty difficulty = Difficulty::Easy;
    std::array<std::array<int, 9>, 9> game{};
    std::array<std::array<int, 9>, 9> initialState{};
    std::array<std::array<int, 9>, 9> solution{};
    void draw_cell(int row, int col, int value) const;
    void init_view() const;
    void draw_grid() const;
    void refresh_view() const;
    void draw_help() const;
};