#pragma once

#include <array>
#include <string>

class Game
{
public:
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
    [[nodiscard]] Cell getCurrentPlayer() const;
    [[nodiscard]] std::string getStatusText() const;

private:
    [[nodiscard]] Cell checkWinner() const;
    [[nodiscard]] bool boardFull() const;

    std::array<std::array<Cell, 3>, 3> m_board{};
    Cell m_currentPlayer{Cell::X};
    Cell m_winner{Cell::Empty};
    bool m_gameOver{false};
};
