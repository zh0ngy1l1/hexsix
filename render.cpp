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

#include <algorithm>
#include <array>
#include <cmath>
#include <optional>

Render::Render()
    : m_window(sf::VideoMode({720, 800}), "6-in-a-row"),
      m_worldView(m_window.getDefaultView())
{
    m_window.setFramerateLimit(60);
    m_fontLoaded = tryLoadSystemFont();
    resetView();
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
                resetView();
            }
            else if (keyPressed->code == sf::Keyboard::Key::Escape)
            {
                m_window.close();
            }
        }

        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mousePressed->button == sf::Mouse::Button::Right ||
                mousePressed->button == sf::Mouse::Button::Middle)
            {
                m_isPanning = true;
                m_lastPanPixel = mousePressed->position;
            }
            else if (mousePressed->button == sf::Mouse::Button::Left)
            {
                const sf::Vector2f worldPos =
                    m_window.mapPixelToCoords(mousePressed->position, m_worldView);
                if (const auto cell = mouseToBoardCell(worldPos))
                {
                    game.handleMove(cell->y, cell->x);
                }
            }
        }

        if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if (mouseReleased->button == sf::Mouse::Button::Right ||
                mouseReleased->button == sf::Mouse::Button::Middle)
            {
                m_isPanning = false;
            }
        }

        if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            if (m_isPanning)
            {
                const sf::Vector2f lastWorld =
                    m_window.mapPixelToCoords(m_lastPanPixel, m_worldView);
                const sf::Vector2f currentWorld =
                    m_window.mapPixelToCoords(mouseMoved->position, m_worldView);
                m_worldView.move(lastWorld - currentWorld);
                m_lastPanPixel = mouseMoved->position;
            }
        }

        if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            if (mouseWheel->wheel == sf::Mouse::Wheel::Vertical)
            {
                const float zoomStep = 1.1f;
                const float rawFactor = std::pow(zoomStep, -mouseWheel->delta);
                const float minZoom = m_defaultZoom * kMinZoomMultiplier;
                const float maxZoom = m_defaultZoom * kMaxZoomMultiplier;
                const float newZoom = std::clamp(m_zoomLevel * rawFactor, minZoom, maxZoom);
                const float appliedFactor = newZoom / m_zoomLevel;
                if (appliedFactor != 1.f)
                {
                    const sf::Vector2f beforeZoom =
                        m_window.mapPixelToCoords(mouseWheel->position, m_worldView);
                    m_worldView.zoom(appliedFactor);
                    m_zoomLevel = newZoom;
                    const sf::Vector2f afterZoom =
                        m_window.mapPixelToCoords(mouseWheel->position, m_worldView);
                    m_worldView.move(beforeZoom - afterZoom);
                }
            }
        }
    }
}

void Render::draw(const Game& game)
{
    m_window.clear(sf::Color(245, 247, 250));

    m_window.setView(m_worldView);
    drawGrid(game);
    drawMarks(game);

    m_window.setView(m_window.getDefaultView());
    drawStatus(game);

    m_window.display();
}

void Render::drawGrid(const Game& game)
{
    const float board = boardSize();
    const float cell = board / static_cast<float>(Game::BOARD_SIZE);

    const float thickness = cell/15.f;
    const sf::Color gridColor(30, 30, 30);

    for (int i = 0; i <= Game::BOARD_SIZE; ++i)
    {
        sf::RectangleShape vLine({thickness, board + thickness});
        vLine.setFillColor(gridColor);
        vLine.setPosition({cell * static_cast<float>(i) - thickness/2.f, -thickness/2.f});
        m_window.draw(vLine);

        sf::RectangleShape hLine({board + thickness, thickness});
        hLine.setFillColor(gridColor);
        hLine.setPosition({-thickness/2.f, cell * static_cast<float>(i) - thickness/2.f});
        m_window.draw(hLine);
    }
}

void Render::drawMarks(const Game& game)
{
    const float board = boardSize();
    const float cell = board / static_cast<float>(Game::BOARD_SIZE);

    for (int row = 0; row < Game::BOARD_SIZE; ++row)
    {
        for (int col = 0; col < Game::BOARD_SIZE; ++col)
        {
            const float centerX = (static_cast<float>(col) + 0.5f) * cell;
            const float centerY = (static_cast<float>(row) + 0.5f) * cell;

            if (game.getCell(row, col) == Game::Cell::X)
            {
                drawX(centerX, centerY, cell * 0.4f);
            }
            else if (game.getCell(row, col) == Game::Cell::O)
            {
                drawO(centerX, centerY, cell * 0.4f);
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

    sf::Text help(m_font, "Left click to play, right/middle drag to pan, wheel to zoom", 19);
    help.setFillColor(sf::Color(90, 90, 90));
    help.setPosition({50.f, 635.f});

    sf::Text help2(m_font, "R to restart, Esc to quit", 19);
    help2.setFillColor(sf::Color(90, 90, 90));
    help2.setPosition({50.f, 660.f});

    m_window.draw(help);
    m_window.draw(help2);
}

void Render::drawX(float centerX, float centerY, float radius)
{
    const float thickness = radius*0.2f;

    sf::RectangleShape lineA({radius*2.f, thickness});
    lineA.setFillColor(sf::Color(220, 50, 47));
    lineA.setOrigin({radius, thickness / 2.f});
    lineA.setPosition({centerX, centerY});
    lineA.setRotation(sf::degrees(45.f));

    sf::RectangleShape lineB({radius*2.f, thickness});
    lineB.setFillColor(sf::Color(220, 50, 47));
    lineB.setOrigin({radius, thickness / 2.f});
    lineB.setPosition({centerX, centerY});
    lineB.setRotation(sf::degrees(-45.f));

    m_window.draw(lineA);
    m_window.draw(lineB);
}

void Render::drawO(float centerX, float centerY, float radius)
{
    const float thickness = radius*0.2f;
    
    radius -= thickness;
    sf::CircleShape ring(radius);
    ring.setFillColor(sf::Color::Transparent);
    ring.setOutlineThickness(thickness);
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

void Render::resetView()
{
    m_worldView = m_window.getDefaultView();
    m_defaultZoom = kReferenceBoardSize / static_cast<float>(Game::BOARD_SIZE);
    m_worldView.zoom(m_defaultZoom);
    m_zoomLevel = m_defaultZoom;
    m_isPanning = false;
}

float Render::boardSize() const
{
    return m_window.getDefaultView().getSize().x;
}

bool Render::pointInBoard(sf::Vector2f point) const
{
    const float x = point.x;
    const float y = point.y;
    const float board = boardSize();

    return x >= 0.f && x < board && y >= 0.f && y < board;
}

std::optional<sf::Vector2i> Render::mouseToBoardCell(sf::Vector2f mousePosition) const
{
    if (!pointInBoard(mousePosition))
    {
        return std::nullopt;
    }

    const float cellSize = boardSize() / static_cast<float>(Game::BOARD_SIZE);
    const int col = static_cast<int>(mousePosition.x / cellSize);
    const int row = static_cast<int>(mousePosition.y / cellSize);

    if (row < 0 || row >= Game::BOARD_SIZE || col < 0 || col >= Game::BOARD_SIZE)
    {
        return std::nullopt;
    }

    return sf::Vector2i{col, row};
}
