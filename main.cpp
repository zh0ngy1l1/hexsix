#include "game.hpp"
#include "render.hpp"

int main()
{
    Game game;
    Render render;
    render.run(game);

    return 0;
}
