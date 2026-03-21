#include "render.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <array>
#include <optional>

Render::Render()
    : m_window(sf::VideoMode({600, 700}), "Tic-Tac-Toe")
{
    m_window.setFramerateLimit(60);
    m_fontLoaded = tryLoadSystemFont();
}

void Render::run(Game& game)
{
    while (m_window.isOpen())
    {
        processEvents(game);
        draw(game);
    }
}

void Render::processEvents(Game& game)
{
    while (const auto event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
            continue;
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->code == sf::Keyboard::Key::R)
            {
                game.reset();
            }
            else if (keyPressed->code == sf::Keyboard::Key::Escape)
            {
                m_window.close();
            }
        }

        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mousePressed->button == sf::Mouse::Button::Left)
            {
                if (const auto cell = mouseToBoardCell(mousePressed->position))
                {
                    game.handleMove(cell->y, cell->x);
                }
            }
        }
    }
}

void Render::draw(const Game& game)
{
    m_window.clear(sf::Color(245, 247, 250));

    drawGrid(game);
    drawMarks(game);
    drawStatus(game);

    m_window.display();
}

void Render::drawGrid(const Game& game)
{
    const float left = kBoardLeft;
    const float top = kBoardTop;
    const float boardSize = kBoardSize;
    const float cell = kBoardSize / 3.f;

    const sf::Color gridColor(30, 30, 30);

    for (int i = 1; i <= 2; ++i)
    {
        sf::RectangleShape vLine({5.f, boardSize});
        vLine.setFillColor(gridColor);
        vLine.setPosition({left + cell * static_cast<float>(i) - 2.5f, top});
        m_window.draw(vLine);

        sf::RectangleShape hLine({boardSize, 5.f});
        hLine.setFillColor(gridColor);
        hLine.setPosition({left, top + cell * static_cast<float>(i) - 2.5f});
        m_window.draw(hLine);
    }
}

void Render::drawMarks(const Game& game)
{
    const float left = kBoardLeft;
    const float top = kBoardTop;
    const float cell = kBoardSize / 3.f;

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            const float centerX = left + (static_cast<float>(col) + 0.5f) * cell;
            const float centerY = top + (static_cast<float>(row) + 0.5f) * cell;

            if (game.getCell(row, col) == Game::Cell::X)
            {
                drawX(centerX, centerY, cell * 0.65f);
            }
            else if (game.getCell(row, col) == Game::Cell::O)
            {
                drawO(centerX, centerY, cell * 0.30f);
            }
        }
    }
}

void Render::drawStatus(const Game& game)
{
    if (!m_fontLoaded)
    {
        return;
    }

    sf::Text text(m_font, game.getStatusText(), 32);
    text.setFillColor(sf::Color(20, 20, 20));
    text.setPosition({50.f, 595.f});

    m_window.draw(text);

    sf::Text help(m_font, "Left click to play, R to restart, Esc to quit", 22);
    help.setFillColor(sf::Color(90, 90, 90));
    help.setPosition({50.f, 645.f});

    m_window.draw(help);
}

void Render::drawX(float centerX, float centerY, float size)
{
    const float thickness = 10.f;

    sf::RectangleShape lineA({size, thickness});
    lineA.setFillColor(sf::Color(220, 50, 47));
    lineA.setOrigin({size / 2.f, thickness / 2.f});
    lineA.setPosition({centerX, centerY});
    lineA.setRotation(sf::degrees(45.f));

    sf::RectangleShape lineB({size, thickness});
    lineB.setFillColor(sf::Color(220, 50, 47));
    lineB.setOrigin({size / 2.f, thickness / 2.f});
    lineB.setPosition({centerX, centerY});
    lineB.setRotation(sf::degrees(-45.f));

    m_window.draw(lineA);
    m_window.draw(lineB);
}

void Render::drawO(float centerX, float centerY, float radius)
{
    sf::CircleShape ring(radius);
    ring.setFillColor(sf::Color::Transparent);
    ring.setOutlineThickness(10.f);
    ring.setOutlineColor(sf::Color(38, 139, 210));
    ring.setOrigin({radius, radius});
    ring.setPosition({centerX, centerY});

    m_window.draw(ring);
}

bool Render::tryLoadSystemFont()
{
    constexpr std::array<const char*, 5> candidates = {
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Tahoma.ttf",
        "/System/Library/Fonts/SFNS.ttf",
        "/Library/Fonts/Arial.ttf",
        "Arial.ttf",
    };

    for (const char* path : candidates)
    {
        if (m_font.openFromFile(path))
        {
            return true;
        }
    }

    return false;
}

bool Render::pointInBoard(sf::Vector2i point) const
{
    const float x = static_cast<float>(point.x);
    const float y = static_cast<float>(point.y);

    return x >= kBoardLeft && x < (kBoardLeft + kBoardSize) && y >= kBoardTop && y < (kBoardTop + kBoardSize);
}

std::optional<sf::Vector2i> Render::mouseToBoardCell(sf::Vector2i mousePosition) const
{
    if (!pointInBoard(mousePosition))
    {
        return std::nullopt;
    }

    const float cellSize = kBoardSize / 3.f;
    const int col = static_cast<int>((static_cast<float>(mousePosition.x) - kBoardLeft) / cellSize);
    const int row = static_cast<int>((static_cast<float>(mousePosition.y) - kBoardTop) / cellSize);

    if (row < 0 || row > 2 || col < 0 || col > 2)
    {
        return std::nullopt;
    }

    return sf::Vector2i{col, row};
}
