#pragma once

#include <SFML/System/Vector2.hpp>

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
    bool handleClick(sf::Vector2i mousePosition);

    [[nodiscard]] Cell getCell(int row, int col) const;
    [[nodiscard]] bool isGameOver() const;
    [[nodiscard]] Cell getWinner() const;
    [[nodiscard]] Cell getCurrentPlayer() const;
    [[nodiscard]] std::string getStatusText() const;

    [[nodiscard]] float boardLeft() const;
    [[nodiscard]] float boardTop() const;
    [[nodiscard]] float boardSize() const;
    [[nodiscard]] float cellSize() const;

private:
    [[nodiscard]] bool pointInBoard(sf::Vector2i point) const;
    [[nodiscard]] Cell checkWinner() const;
    [[nodiscard]] bool boardFull() const;

    std::array<std::array<Cell, 3>, 3> m_board{};
    Cell m_currentPlayer{Cell::X};
    Cell m_winner{Cell::Empty};
    bool m_gameOver{false};

    static constexpr float kBoardLeft = 50.f;
    static constexpr float kBoardTop = 50.f;
    static constexpr float kBoardSize = 500.f;
};
