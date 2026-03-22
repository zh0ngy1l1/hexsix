#include "game.hpp"

Game::Game()
{
    reset();
}

void Game::reset()
{
    m_boardX.reset();
    m_boardO.reset();
    m_turn = 1;
    m_winner = Cell::Empty;
    m_gameOver = false;
}

bool Game::handleMove(int row, int col)
{
    if (m_gameOver)
    {
        return false;
    }

    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
    {
        return false;
    }

    const int cellIdx = BOARD_SIZE * row + col;
    if (m_boardX[cellIdx] || m_boardO[cellIdx])
    {
        return false;
    }

    const bool xTurn = isXTurn();
    std::bitset<BOARD_CELLS>& curr_board = xTurn ? m_boardX : m_boardO;
    curr_board[cellIdx] = 1;

    m_winner = detectWinner(cellIdx, curr_board);
    if (m_winner != Cell::Empty || boardFull())
    {
        m_gameOver = true;
    }
    else
    {
        ++m_turn;
    }

    return true;
}

Game::Cell Game::getCell(int row, int col) const
{
    const int cellIdx = BOARD_SIZE * row + col;
    if (m_boardX[cellIdx])
    {
        return Cell::X;
    }
    if (m_boardO[cellIdx])
    {
        return Cell::O;
    }
    return Cell::Empty;
}

bool Game::isGameOver() const
{
    return m_gameOver;
}

Game::Cell Game::getWinner() const
{
    return m_winner;
}

bool Game::isXTurn() const
{
    return (m_turn % 4) < 2;
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

    return isXTurn() ? "Turn: X" : "Turn: O";
}

bool Game::boardFull() const
{
    return (m_boardX | m_boardO).count() == BOARD_CELLS;
}

Game::Cell Game::detectWinner(int cellIdx, const std::bitset<BOARD_CELLS>& curr_board) const
{
    const int row = cellIdx / BOARD_SIZE;
    const int col = cellIdx % BOARD_SIZE;

    auto hasRunThrough = [&](int dr, int dc) -> bool
    {
        int count = 1;

        int r = row + dr;
        int c = col + dc;
        while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE &&
               curr_board[BOARD_SIZE * r + c])
        {
            ++count;
            r += dr;
            c += dc;
        }

        r = row - dr;
        c = col - dc;
        while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE &&
               curr_board[BOARD_SIZE * r + c])
        {
            ++count;
            r -= dr;
            c -= dc;
        }

        return count >= WIN_LENGTH;
    };

    if (hasRunThrough(0, 1) || hasRunThrough(1, 0) || hasRunThrough(1, 1))
    {
        return (&curr_board == &m_boardX) ? Cell::X : Cell::O;
    }

    return Cell::Empty;
}
