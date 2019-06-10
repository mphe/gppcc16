#include "gamelib/Engine.hpp"
#include "gamelib/core/Game.hpp"
#include "gamelib/editor/Editor.hpp"
#include "BoxerController.hpp"
#include "HealthComponent.hpp"
#include "TargetComponent.hpp"
#include "EnduranceComponent.hpp"
#include "BoxerAI.hpp"
#include "Gui.hpp"
#include "Fader.hpp"
#include "Logic.hpp"
#include "gamelib/export.hpp"
#include <cstring>
#include <SFML/Graphics.hpp>

using namespace gamelib;
using namespace gppcc16;

int main(int argc, char *argv[])
{
    Game game;
    GameStatePtr engineState(new Engine(false));
    auto engine = static_cast<Engine*>(engineState.get());

    engine->entfactory.addComponent<BoxerController>();
    engine->entfactory.addComponent<HealthComponent>();
    engine->entfactory.addComponent<TargetComponent>();
    engine->entfactory.addComponent<EnduranceComponent>();
    engine->entfactory.addComponent<BoxerAI>();
    engine->entfactory.addComponent<Fader>();
    engine->entfactory.addComponent<Logic>();

    engine->resmgr.loadFromFile("assets/res.json");
    game.loadFromFile("assets/game.json");
    game.init();
    game.pushState(std::move(engineState));
    game.pushState(GameStatePtr(new GuiState()));

    if (argc > 1 && strcmp(argv[1], "editor") == 0)
    {
        game.pushState(GameStatePtr(new Editor()));

        if (argc > 2 && strlen(argv[2]) > 0)
            static_cast<Editor*>(game.pullState())->load(argv[2]);
    }
    else
    {
        gamelib::loadSave("assets/map.json");
        game.getWindow().setMouseCursorVisible(false);
    }

    game.run();
    game.destroy();

    return 0;
}
