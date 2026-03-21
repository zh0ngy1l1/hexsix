#pragma once

#include "game.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
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

    void drawX(float centerX, float centerY, float size);
    void drawO(float centerX, float centerY, float radius);

    [[nodiscard]] bool pointInBoard(sf::Vector2i point) const;
    [[nodiscard]] std::optional<sf::Vector2i> mouseToBoardCell(sf::Vector2i mousePosition) const;
    bool tryLoadSystemFont();

    sf::RenderWindow m_window;
    sf::Font m_font;
    bool m_fontLoaded{false};
    std::string m_status;

    static constexpr float kBoardLeft = 50.f;
    static constexpr float kBoardTop = 50.f;
    static constexpr float kBoardSize = 500.f;
};
