#pragma once

#include "game.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#include <optional>
#include <string>

class Render
{
public:
    Render();

    void run(Game& game);

private:
    void processEvents(Game& game);
    void draw(const Game& game);

    void drawGrid(const Game& game);
    void drawMarks(const Game& game);
    void drawStatus(const Game& game);

    void drawX(float centerX, float centerY, float radius);
    void drawO(float centerX, float centerY, float radius);
    void resetView();
    [[nodiscard]] float boardSize() const;

    [[nodiscard]] bool pointInBoard(sf::Vector2f point) const;
    [[nodiscard]] std::optional<sf::Vector2i> mouseToBoardCell(sf::Vector2f mousePosition) const;
    bool tryLoadSystemFont();

    sf::RenderWindow m_window;
    sf::View m_worldView;
    sf::Font m_font;
    bool m_fontLoaded{false};
    bool m_isPanning{false};
    float m_defaultZoom{1.f};
    float m_zoomLevel{1.f};
    sf::Vector2i m_lastPanPixel{0, 0};
    std::string m_status;

    static constexpr float kReferenceBoardSize = 15.f;
    static constexpr float kMinZoomMultiplier = 0.5f;
    static constexpr float kMaxZoomMultiplier = 3.f;
};
