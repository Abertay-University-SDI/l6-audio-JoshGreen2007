#pragma once
#include "Framework/BaseLevel.h"
#include "Framework/Collision.h"
class Menu :
    public BaseLevel
{
public:
    Menu(sf::RenderWindow& window, Input& input, GameState& state, AudioManager& audio);

    void handleInput(float dt) override;
    void update(float dt) override {};  // no updating tyvm
    void render() override;
    void reset();

private:
    sf::Font m_font;
    sf::Text m_prompt;

};

