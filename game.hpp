#pragma once

#include <bitset>
#include <string>

class Game
{
public:
    static constexpr int BOARD_SIZE = 20;
    static constexpr int WIN_LENGTH = 6;
    static constexpr int BOARD_CELLS = BOARD_SIZE * BOARD_SIZE;

    enum class Cell
    {
        Empty,
        X,
        O
    };

    Game();

    void reset();
    bool handleMove(int row, int col);

    [[nodiscard]] Cell getCell(int row, int col) const;
    [[nodiscard]] bool isGameOver() const;
    [[nodiscard]] Cell getWinner() const;
    [[nodiscard]] bool isXTurn() const;
    [[nodiscard]] std::string getStatusText() const;

private:
    [[nodiscard]] Cell detectWinner(int cellIdx, const std::bitset<BOARD_CELLS>& curr_board) const;
    [[nodiscard]] bool boardFull() const;

    std::bitset<BOARD_CELLS> m_boardX{0};
    std::bitset<BOARD_CELLS> m_boardO{0};
    int m_turn{1};
    Cell m_winner{Cell::Empty};
    bool m_gameOver{false};
};
