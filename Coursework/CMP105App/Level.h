#pragma once

#include "Framework/BaseLevel.h"
#include "Framework/GameObject.h"
#include "Framework/Collision.h"
#include "Sheep.h"
#include "Rabbit.h"

#include <iomanip>      // Phase one includes
#include <fstream>

class Level : BaseLevel {
public:
	Level(sf::RenderWindow& window, Input& input, GameState& state, AudioManager& audio);
	~Level() override;

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

    void reset();

private:
    void UpdateCamera();
    void manageCollisions();

    void writeHighScore(float timeTaken);  
    void displayScoreboard();

    void loadLevel(std::string fileName, sf::Vector2f worldSize); 

    sf::View m_cameraView;

    // Entities
    Rabbit* m_playerRabbit;
    std::vector<Sheep*> m_sheepList;
    std::vector<GameObject> m_walls; 
    GameObject m_goal;
    sf::Texture m_sheepTexture;
    sf::Texture m_rabbitTexture;
    GameObject m_bgFarm;

    // Level Data
    sf::FloatRect m_levelBounds;
    bool m_isGameOver;

    // UI & Timer
    sf::Clock m_gameTimer;      // We will replace this with a float, although clocks are cool.
    sf::Font m_font;
    sf::Text m_timerText;
    sf::Text m_winText;
	
    sf::Text m_scoreboardText;  
    std::string m_levelName;

};