#include "Level.h"

Level::Level(sf::RenderWindow& hwnd, Input& in, GameState& gs, AudioManager& audio) :
    BaseLevel(hwnd, in, gs, audio), m_timerText(m_font), m_winText(m_font), m_scoreboardText(m_font)
{
    if (!m_font.openFromFile("font/arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }

    // expensive file i/o we will do only once
    if (!m_rabbitTexture.loadFromFile("gfx/rabbit_sheet.png")) std::cerr << "no rabbit texture";
    if (!m_sheepTexture.loadFromFile("gfx/sheep_sheet.png")) std::cerr << "no sheep texture";

    // everything else we can chuck into reset()
    m_playerRabbit = nullptr;    // ensures nothing is deleted inside reset();
    reset();   
}

Level::~Level()
{
    // We made a lot of **new** things, allocating them on the heap
    // So now we have to clean them up
    delete m_playerRabbit;
    for (Sheep* s : m_sheepList) delete s;
    m_sheepList.clear();
}

void Level::reset()
{
    sf::Vector2f levelSize = { 800.f, 800.f };
    m_levelBounds = { {0.f, 0.f}, levelSize };
    m_isGameOver = false;
    if (m_playerRabbit) delete m_playerRabbit;
    m_playerRabbit = new Rabbit();
    
    m_playerRabbit->setSize({ 32.f,32.f });
    m_playerRabbit->setInput(&m_input);
    m_playerRabbit->setTexture(&m_rabbitTexture);
    m_playerRabbit->setWorldSize(levelSize.x, levelSize.y);

    m_timerText.setFont(m_font);
    m_timerText.setCharacterSize(24);
    m_timerText.setFillColor(sf::Color::White);

    // "Game Over" text setup
    m_winText.setFont(m_font);
    m_winText.setString("ROUND COMPLETE!");
    m_winText.setCharacterSize(50);
    m_winText.setFillColor(sf::Color::Blue);
    m_winText.setPosition({ -1000.f, 100.f });

    m_bgFarm.setFillColor(sf::Color::Green);
    m_bgFarm.setSize(levelSize);
    m_scoreboardText.setFont(m_font);

    m_walls.clear();
    for (Sheep* s : m_sheepList)
    {
        delete s;
    }
    m_sheepList.clear();

    loadLevel("data/level1.txt", levelSize);

    m_timeSpent = 0.f;
    m_sheepTimer = 0.f;

    m_audio.stopAllMusic();
    m_audio.playMusicbyName("nature");

}

void Level::UpdateCamera()
{
    sf::View view = m_window.getView();
    sf::Vector2f targetPos = m_playerRabbit->getPosition();

    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f halfSize = { viewSize.x / 2.f, viewSize.y / 2.f };

    sf::Vector2f newCenter;

    newCenter.x = std::clamp(targetPos.x,
        m_levelBounds.position.x + halfSize.x,
        m_levelBounds.size.x - halfSize.x);

    newCenter.y = std::clamp(targetPos.y,
        m_levelBounds.position.y + halfSize.y,
        m_levelBounds.size.y - halfSize.y);

    view.setCenter(newCenter);

    // top-left corner of the current view in world space
    sf::Vector2f viewTopLeft = newCenter - halfSize;
    // Position text at the bottom-left of the current view with a small margin (e.g., 30px)
    float margin = 30.f;
    m_timerText.setPosition({ viewTopLeft.x + margin, viewTopLeft.y + viewSize.y - margin });

    m_window.setView(view);
}

void Level::spawnSheep()
{

    // Random position inside level bounds
    int maxX = std::rand() % static_cast<int>(m_levelBounds.size.x - 32);
    int maxY = std::rand() % static_cast<int>(m_levelBounds.size.y - 32);

    sf::Vector2f sheepPos;
    bool validPos = false;

    while (!validPos)
    {

        sheepPos = {

            // Conversion type int -> float required for randomisation
            static_cast<float>(rand() % maxX),
            static_cast<float > (rand() % maxY)
            
        };

        validPos = checkPositionOutsideWalls(sheepPos);

    }

    Sheep* newSheep = new Sheep(sheepPos, m_playerRabbit);
    newSheep->setAudioPointer(&m_audio);
    newSheep->setTexture(&m_sheepTexture);
    newSheep->setSize({ 32.f,32.f });
    newSheep->setWorldSize(m_levelBounds.size.x, m_levelBounds.size.y);
    newSheep->setAudioPointer(&m_audio);
    m_sheepList.push_back(newSheep);

}

bool Level::checkPositionOutsideWalls(sf::Vector2f pos)
{

    sf::Vector2i checkPos({
        static_cast<int>(pos.x),
        static_cast<int>(pos.y)
        });

    for (auto& wall : m_walls)
        if (Collision::checkBoundingBox(wall, checkPos)) return false;
    return true;

}

// handle user input
void Level::handleInput(float dt)
{
    if (m_input.isPressed(sf::Keyboard::Scancode::Escape))
        m_gameState.setCurrentState(State::MENU);
    m_playerRabbit->handleInput(dt);
}

// checks and manages sheep-sheep, sheep-wall, sheep-goal, player-wall
void Level::manageCollisions()
{
    for (int i = 0; i < m_sheepList.size(); i++)
    {
        if (!m_sheepList[i]->isAlive()) continue;   // ignore scored sheep.

        // sheep collide with walls, with other sheep and with the goal.
        for (auto wall : m_walls)
        {
            if (Collision::checkBoundingBox(wall, *m_sheepList[i]))
            {
                m_sheepList[i]->collisionResponse(wall);
            }
        }
        for (int j = i + 1; j < m_sheepList.size(); j++)
        {
            if (!m_sheepList[j]->isAlive()) continue; // ignore scored sheep here too
            if (Collision::checkBoundingBox(*m_sheepList[i], *m_sheepList[j]))
            {
                // DANGER check i and j carefully here team.
                m_sheepList[i]->collisionResponse(*m_sheepList[j]);
                m_sheepList[j]->collisionResponse(*m_sheepList[i]);
            }
        }
        if (Collision::checkBoundingBox(*m_sheepList[i], m_goal))
        {

            m_sheepList[i]->collideWithGoal(m_goal);
            m_audio.playSoundbyName("yay");

        }
    }
    for (auto wall : m_walls)
    {
        if (Collision::checkBoundingBox(wall, *m_playerRabbit))
        {
            m_playerRabbit->collisionResponse(wall);
        }
    }
}

// Update game objects
void Level::update(float dt)
{

    if (m_isGameOver) return;   // if the game is over, don't continue trying to process game logic

    m_playerRabbit->update(dt);
    for (Sheep* s : m_sheepList) if (s->isAlive()) s->update(dt);

    m_sheepTimer -= dt;

    if (m_sheepTimer <= 0.f)
    {

        spawnSheep();
        m_sheepTimer = SHEEP_INTERVAL;

    }


    m_timeSpent += dt;

    float timeReamining = m_maxTime - m_timeSpent;

    // Convert float text into string
    m_timerText.setString("Time: " + std::to_string(static_cast<int>(timeReamining)));

    if (m_timeSpent < 0.f) m_timeSpent = 0.f;

    // Game Over is active when timer reaches maxTime
    m_isGameOver = (m_timeSpent >= m_maxTime);

    manageCollisions();
    UpdateCamera();

    if (m_isGameOver)
    {
        // will happen ONCE in the frame that game over is triggered.
        writeHighScore(m_timeSpent);
        displayScoreboard();
    }

}

void Level::displayHUD()
{

    sf::View old_view = m_window.getView();
    sf::Vector2f middle = old_view.getCenter();

    float half_dim = old_view.getSize().x / 2.f; // Assumed square view

    m_window.setView(m_window.getDefaultView());

    for (auto& sheep : m_sheepList)
    {

        // If the Sheep is dead, skip this operation
        if (!sheep->isAlive()) continue;

        float markerX = half_dim;
        float markerY = half_dim;

        if (sheep->getPosition().x < middle.x - half_dim) markerX = 2.f; // Slightly outside
        if (sheep->getPosition().x > middle.x + half_dim)
        {

            markerX = old_view.getSize().x - 12.f;

        }

        if (sheep->getPosition().y < middle.y - half_dim) markerY = 2.f; // Slightly outside
        if (sheep->getPosition().y > middle.y + half_dim)
        {

            markerY = old_view.getSize().y - 12.f;

        }

        // If Sheep is on the screen, skip (I.E don't bother displaying as we can see the Sheep
        if (markerX == half_dim && markerY == half_dim) continue;

        sf::CircleShape marker;
        marker.setRadius(5.f);
        marker.setPosition({ markerX, markerY });
        marker.setFillColor(sf::Color::Red);
        m_window.draw(marker);

    }

    // Reset view back to old view
    m_window.setView(old_view);

}

// Render level
void Level::render()
{
    beginDraw();
    m_window.draw(m_bgFarm);
    m_window.draw(m_goal);
    for (auto w : m_walls) m_window.draw(w);
    for (auto s : m_sheepList)
    {
        if (s->isAlive()) m_window.draw(*s);
    }
    m_window.draw(*m_playerRabbit);
    m_window.draw(m_timerText);

    displayHUD();

    if (m_isGameOver)
    {
        m_window.draw(m_winText);
        m_window.draw(m_scoreboardText);
    }
    endDraw();
}

void Level::writeHighScore(float timeTaken)
{
    std::ofstream outputFile("data/scoreboard.txt", std::ios::app);

    if (outputFile.is_open())
    {
        outputFile << std::fixed << std::setprecision(2) << timeTaken << "\n";
        outputFile.close();
    }
}

void Level::displayScoreboard()
{
    std::ifstream inputFile("data/scoreboard.txt");
    std::string line;
    std::string fullText = "Highscores:\n";
    while (std::getline(inputFile, line))
    {
        fullText += line + "\n";
    }
    inputFile.close();

    m_scoreboardText.setString(fullText);
    m_scoreboardText.setCharacterSize(24);
    m_scoreboardText.setFillColor(sf::Color::Black);
    m_scoreboardText.setPosition({ 400.f,200.f });
}

void Level::loadLevel(std::string fileName, sf::Vector2f worldSize)
{
    std::ifstream inputFile(fileName);
    std::string object;
    float x;
    float y;
    float w;
    float h;

    // while I can read a line which is "string float float"
    while (inputFile >> object)
    {
        if (object == "SHEEP")
        {
            inputFile >> x >> y;
            Sheep* newSheep = new Sheep(sf::Vector2f(x, y), m_playerRabbit);
            newSheep->setAudioPointer(&m_audio);
            newSheep->setTexture(&m_sheepTexture);
            newSheep->setSize({ 32.f,32.f });
            newSheep->setWorldSize(worldSize.x, worldSize.y);
            m_sheepList.push_back(newSheep);
        }
        else if (object == "RABBIT")
        {
            inputFile >> x >> y;
            m_playerRabbit->setPosition({ x, y });
        }
        else if (object == "GOAL")
        {
            inputFile >> x >> y;
            m_goal.setSize({ 50.f, 50.f });
            m_goal.setFillColor(sf::Color::Blue);
            m_goal.setPosition({ x, y });
            m_goal.setCollisionBox({ { 0.f,0.f }, { 50.f,50.f } });

        }
        else if (object == "WALL")
        {
            inputFile >> x >> y >> w >> h;
            GameObject wall;
            wall.setPosition({ x, y });
            wall.setSize({ w,h });
            wall.setFillColor(sf::Color::Black);
            wall.setCollisionBox({ { 0.f,0.f }, { w,h } });
            m_walls.push_back(wall);
        }
        else
        {
            std::cout << "Type " << object << " found, but not recognised\n";
        }
    }
}


