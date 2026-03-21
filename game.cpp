#include "game.hpp"

Game::Game()
{
    reset();
}

void Game::reset()
{
    for (auto& row : m_board)
    {
        row.fill(Cell::Empty);
    }

    m_currentPlayer = Cell::X;
    m_winner = Cell::Empty;
    m_gameOver = false;
}

bool Game::handleClick(sf::Vector2i mousePosition)
{
    if (m_gameOver || !pointInBoard(mousePosition))
    {
        return false;
    }

    const int col = static_cast<int>((static_cast<float>(mousePosition.x) - kBoardLeft) / cellSize());
    const int row = static_cast<int>((static_cast<float>(mousePosition.y) - kBoardTop) / cellSize());

    if (row < 0 || row > 2 || col < 0 || col > 2 || m_board[row][col] != Cell::Empty)
    {
        return false;
    }

    m_board[row][col] = m_currentPlayer;

    m_winner = checkWinner();
    if (m_winner != Cell::Empty)
    {
        m_gameOver = true;
    }
    else if (boardFull())
    {
        m_gameOver = true;
    }
    else
    {
        m_currentPlayer = (m_currentPlayer == Cell::X) ? Cell::O : Cell::X;
    }

    return true;
}

Game::Cell Game::getCell(int row, int col) const
{
    return m_board[row][col];
}

bool Game::isGameOver() const
{
    return m_gameOver;
}

Game::Cell Game::getWinner() const
{
    return m_winner;
}

Game::Cell Game::getCurrentPlayer() const
{
    return m_currentPlayer;
}

std::string Game::getStatusText() const
{
    if (m_winner == Cell::X)
    {
        return "Winner: X  (Press R to restart)";
    }

    if (m_winner == Cell::O)
    {
        return "Winner: O  (Press R to restart)";
    }

    if (m_gameOver)
    {
        return "Draw  (Press R to restart)";
    }

    return (m_currentPlayer == Cell::X) ? "Turn: X" : "Turn: O";
}

float Game::boardLeft() const
{
    return kBoardLeft;
}

float Game::boardTop() const
{
    return kBoardTop;
}

float Game::boardSize() const
{
    return kBoardSize;
}

float Game::cellSize() const
{
    return kBoardSize / 3.f;
}

bool Game::pointInBoard(sf::Vector2i point) const
{
    const float x = static_cast<float>(point.x);
    const float y = static_cast<float>(point.y);

    return x >= kBoardLeft && x < (kBoardLeft + kBoardSize) && y >= kBoardTop && y < (kBoardTop + kBoardSize);
}

Game::Cell Game::checkWinner() const
{
    auto lineWinner = [](Cell a, Cell b, Cell c) -> Cell {
        if (a != Cell::Empty && a == b && b == c)
        {
            return a;
        }
        return Cell::Empty;
    };

    for (int i = 0; i < 3; ++i)
    {
        if (const Cell rowWinner = lineWinner(m_board[i][0], m_board[i][1], m_board[i][2]); rowWinner != Cell::Empty)
        {
            return rowWinner;
        }

        if (const Cell colWinner = lineWinner(m_board[0][i], m_board[1][i], m_board[2][i]); colWinner != Cell::Empty)
        {
            return colWinner;
        }
    }

    if (const Cell diagWinner = lineWinner(m_board[0][0], m_board[1][1], m_board[2][2]); diagWinner != Cell::Empty)
    {
        return diagWinner;
    }

    return lineWinner(m_board[0][2], m_board[1][1], m_board[2][0]);
}

bool Game::boardFull() const
{
    for (const auto& row : m_board)
    {
        for (const Cell cell : row)
        {
            if (cell == Cell::Empty)
            {
                return false;
            }
        }
    }

    return true;
}
