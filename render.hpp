#pragma once

#include "game.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

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

    bool tryLoadSystemFont();

    sf::RenderWindow m_window;
    sf::Font m_font;
    bool m_fontLoaded{false};
    std::string m_status;
};
