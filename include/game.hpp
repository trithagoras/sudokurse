#pragma once
#include <array>
#include <string>
#include "stopwatch.hpp"
#include <memory>

namespace GameConstants {
    constexpr int cellHeight = 2;
    constexpr int cellWidth = 4;
    constexpr int unset = 0;
    constexpr int lightColorPair = 1;
    constexpr int COLOR_GRAY = 8;
    constexpr int cursorColorPair = 2;
    constexpr int yellowColorPair = 3;
    constexpr int redColorPair = 4;
    constexpr int greenColorPair = 5;
    constexpr int offsetY = 1;
    constexpr int offsetX = 1;
}

enum class Difficulty {
    Easy,
    Medium,
    Hard,
    Master
};

std::string difficulty_str(Difficulty d);

class Game {
public:
    Game();
    ~Game();
    void start();
private:
    int cursorY = 0, cursorX = 0;
    Difficulty difficulty = Difficulty::Easy;
    std::array<std::array<int, 9>, 9> game{};
    std::array<std::array<int, 9>, 9> penciled1{};
    std::array<std::array<int, 9>, 9> penciled2{};
    std::array<std::array<int, 9>, 9> initialState{};
    std::array<std::array<int, 9>, 9> solution{};
    std::string errorText;
    std::string successText;
    std::unique_ptr<Stopwatch> stopwatch;
    bool isPencil2 = false;
    void reset_game();
    void update_loop();
    int at(int row, int col) const;
    void set(int row, int col, int value, bool isPenciled = false);
    void set_at_cursor(int value, bool isPenciled = false);
    void clear(int row, int col, bool isPenciled = false);
    void clear_at_cursor(bool isPenciled = false);
    int at_cursor() const;
    void translate_cursor(int dRow, int dCol);
    void move_cursor(int row, int col);
    void try_solve();
    void auto_solve();
    void draw_cell(int row, int col) const;
    void init_view() const;
    void draw_grid() const;
    void refresh_view();
    void draw_help() const;
};