#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>

// #include "spriteClass.h"
/**
 * @brief the main loop for Timber game.
 *
 * @return int
 */
using namespace sf;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
const int NUM_CLOUDS = 3;
const float AXE_POS_LEFT = 497;
const float AXE_POS_RIGHT = 763;

enum class side
{
    LEFT,
    RIGHT,
    NONE
};

side branchPostition[NUM_BRANCHES];

Sprite branches[NUM_BRANCHES];

class CustomSprite
{
public:
    Texture texture;
    Sprite sprite;
    float speed;
    bool active;
};

int main(void)
{

    VideoMode vm = VideoMode::getDesktopMode();
    RenderWindow window(vm, "Timber", Style::Fullscreen);

    Texture texBackground;
    Texture texTree;
    Texture texBee;
    Texture texBranch;
    Texture texTimebarBack;
    Texture texTimebarFront;

    texBackground.loadFromFile("graphics/background.png");
    texTree.loadFromFile("graphics/tree.png");
    texBee.loadFromFile("graphics/bee.png");
    texBranch.loadFromFile("graphics/branch.png");
    texTimebarBack.loadFromFile("graphics/1_Back.png");
    texTimebarFront.loadFromFile("graphics/1_Front.png");

    Sprite spriteBackground;
    Sprite spriteTree;
    Sprite spriteBee;
    Sprite spriteBranch;

    std::vector<CustomSprite> spriteClouds(NUM_CLOUDS);

    for (int i = 0; i < spriteClouds.size(); i++)
    {
        srand(int(time(0)));
        spriteClouds[i].texture.loadFromFile("graphics/cloud.png");
        spriteClouds[i].sprite.setTexture(spriteClouds[i].texture);
        spriteClouds[i].active = false;
        spriteClouds[i].speed = 0.0f;
    }

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(texBranch);
        branches[i].setPosition(vm.width / 2.0f, -200);
        branches[i].setScale(0.71f, 0.71f);
        branches[i].setOrigin(220, 20);
    }
    spriteBee.setTexture(texBee);
    spriteBee.setPosition(vm.width + 300, 100);
    spriteBackground.setTexture(texBackground);
    spriteBackground.setPosition(0, 0);
    spriteBackground.setScale(0.71f, 0.71f);

    spriteTree.setTexture(texTree);
    spriteTree.setPosition(575, 0);
    spriteTree.setScale(0.71f, 0.71f);

    bool beeActive = false;
    float beeSpeed = 0.0f;
    /**
     *
     * @brief The main game loop is below
     * we add the clock and pause variables
     * below also, and anything else we might
     * need over time.
     *
     */
    bool paused = true;
    Clock clock;

    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    Text gameText;
    gameText.setString("Press enter to play.");
    gameText.setCharacterSize(75);
    gameText.setFillColor(Color::Blue);
    gameText.setFont(font);
    FloatRect gameTextRect = gameText.getLocalBounds();
    gameText.setOrigin(gameTextRect.left + gameTextRect.width / 2.0f,
                       gameTextRect.top + gameTextRect.height / 2.0f);
    gameText.setPosition(vm.width / 2.0f, vm.height / 2.0f);

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(Color::Blue);
    scoreText.setString("Score = 0");
    scoreText.setPosition(20, 20);

    int score = 0;
    std::string baseScoreText = "Score = ";

    RectangleShape timebarFront;
    RectangleShape timebarBack;
    float timebarStartWidth = 300;
    float timebarHeight = 45;
    timebarBack.setSize(Vector2f(timebarStartWidth, timebarHeight));
    timebarBack.setTexture(&texTimebarBack);
    timebarBack.setPosition((vm.width / 2) - timebarStartWidth / 2, (vm.height - timebarHeight) - 15);
    timebarFront.setSize(Vector2f(timebarStartWidth, timebarHeight));
    timebarFront.setTexture(&texTimebarFront);
    timebarFront.setPosition((vm.width / 2) - timebarStartWidth / 2, (vm.height - timebarHeight) - 15);

    Time gameTimeTotal;
    float gameTimeRemaining = 6.0f;
    float timebarWPS = timebarStartWidth / gameTimeRemaining;

    CustomSprite player;
    player.texture.loadFromFile("graphics/player.png");
    player.sprite.setTexture(player.texture);
    side playerside = side::LEFT;
    player.sprite.setPosition(412, 511);
    player.sprite.setScale(0.71f, 0.71f);

    CustomSprite gravestone;
    gravestone.texture.loadFromFile("graphics/rip.png");
    gravestone.sprite.setTexture(gravestone.texture);
    gravestone.sprite.setPosition(426, 611);
    gravestone.sprite.setScale(0.71f, 0.71f);

    CustomSprite axe;
    axe.texture.loadFromFile("graphics/axe.png");
    axe.sprite.setTexture(axe.texture);
    axe.sprite.setPosition(497, 589);
    axe.sprite.setScale(0.71f, 0.71f);

    CustomSprite log;
    log.texture.loadFromFile("graphics/log.png");
    log.sprite.setTexture(log.texture);
    log.sprite.setPosition(575, 511);
    log.sprite.setScale(0.71f, 0.71f);
    bool logActive = false;
    float logSpeedX = 710;
    float logSpeedY = -1065;
    bool acceptInput = false;

    /**
     * @brief Start of game loop!
     *
     */

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyPressed)
            {
                if (Keyboard::isKeyPressed(Keyboard::Escape))
                {
                    window.close();
                }
                else if (Keyboard::isKeyPressed(Keyboard::Return))
                {
                    paused = false;
                    score = 0;
                    gameTimeRemaining = 6.0f;
                    for (int i = 1; i < NUM_BRANCHES; i++)
                    {
                        branchPostition[i] = side::NONE;
                    }
                    gravestone.sprite.setPosition(675, 2000);
                    player.sprite.setPosition(412, 511);
                    acceptInput = true;
                }
                else if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
                {
                    if (acceptInput)
                    {
                        playerside = side::LEFT;
                        score++;
                        gameTimeRemaining += (2 / score) + 0.15f;
                        axe.sprite.setPosition(AXE_POS_LEFT, axe.sprite.getPosition().y);
                        player.sprite.setPosition(412, player.sprite.getPosition().y);
                        updateBranches(score);
                        log.sprite.setPosition(575, 511);
                        logSpeedX = +5000;
                        logActive = true;
                        acceptInput = false;
                    }
                }
                else if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
                {
                    if (acceptInput)
                    {
                        playerside = side::RIGHT;
                        score++;
                        gameTimeRemaining += (2 / score) + 0.15f;
                        axe.sprite.setPosition(AXE_POS_RIGHT, axe.sprite.getPosition().y);
                        player.sprite.setPosition(852, player.sprite.getPosition().y);
                        updateBranches(score);
                        log.sprite.setPosition(575, 511);
                        logSpeedX = -5000;
                        logActive = true;
                        acceptInput = false;
                    }
                }
            }
            else if (event.type == Event::KeyReleased && !paused)
            {
                acceptInput = true;
                axe.sprite.setPosition(2000, axe.sprite.getPosition().y);
            }
        }

        if (!paused)
        {
            Time dt = clock.restart();
            gameTimeRemaining -= dt.asSeconds();
            if (gameTimeRemaining <= 0.0f)
            {
                paused = true;
                gameText.setString("Game Over");
                gameTextRect = gameText.getLocalBounds();
                gameText.setOrigin(gameTextRect.left + gameTextRect.width / 2.0f, gameTextRect.top + gameTextRect.height / 2.0f);
                gameText.setPosition(vm.width / 2.0f, vm.height / 2.0f);
                gameTimeRemaining = 6.0f;
            }
            timebarFront.setSize(Vector2f(timebarWPS * gameTimeRemaining, timebarHeight));

            if (!beeActive)
            {
                srand(int(time(0)));
                beeSpeed = (rand() % 200) + 200;

                srand(int(time(0)) * 10);

                float height = (rand() % 384) + 360;
                spriteBee.setPosition(vm.width + 300, height);
                beeActive = true;
            }
            else
            {
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                                      spriteBee.getPosition().y);
                if (spriteBee.getPosition().x < -100)
                {
                    spriteBee.setPosition(vm.width + 300, 0);
                    beeActive = false;
                }
            }

            for (int i = 0; i < spriteClouds.size(); i++)
            {
                if (!spriteClouds[i].active)
                {
                    srand(int(time(0)) * (10 * (i + 1)));
                    spriteClouds[i].speed = (rand() % 50) + 200;
                    srand(int(time(0)) * (20 * (i + 1)));
                    spriteClouds[i].sprite.setPosition(-150 * (i + 1), (rand() % 100) + (i * 150));
                    spriteClouds[i].active = true;
                }
                else
                {
                    spriteClouds[i].sprite.setPosition(spriteClouds[i].sprite.getPosition().x +
                                                           (spriteClouds[i].speed * dt.asSeconds()),
                                                       spriteClouds[i].sprite.getPosition().y);
                    if (spriteClouds[i].sprite.getPosition().x > vm.width + 50)
                    {
                        spriteClouds[i].sprite.setPosition(-350, 0);
                        spriteClouds[i].active = false;
                    }
                }
            }
            std::stringstream ss;
            ss << baseScoreText << score;
            scoreText.setString(ss.str());

            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 107;
                if (branchPostition[i] == side::LEFT)
                {
                    branches[i].setPosition(434, height);
                    branches[i].setRotation(180);
                }
                else if (branchPostition[i] == side::RIGHT)
                {
                    branches[i].setPosition(944, height);
                    branches[i].setRotation(0);
                }
                else
                {
                    branches[i].setPosition(3000, 3000);
                }
            }
            if (logActive)
            {
                log.sprite.setPosition(log.sprite.getPosition().x + (logSpeedX * dt.asSeconds()),
                                       log.sprite.getPosition().y + (logSpeedY * dt.asSeconds()));
                if (log.sprite.getPosition().x < -100 || log.sprite.getPosition().x > vm.width + 100)
                {
                    logActive = false;
                    log.sprite.setPosition(575, 511);
                }
            }
            if (branchPostition[NUM_BRANCHES - 1] == playerside)
            {
                paused = true;
                acceptInput = false;
                gravestone.sprite.setPosition(player.sprite.getPosition().x, 611);
                player.sprite.setPosition(vm.width + 300, 611);
                gameText.setString("Squished!");
                gameTextRect = gameText.getLocalBounds();
                gameText.setOrigin((gameTextRect.left + gameTextRect.width) / 2.0f, (gameTextRect.top + gameTextRect.height) / 2.0f);
                gameText.setPosition(vm.width / 2.0f, vm.height / 2.0f);
            }
        } // End not paused
        // Draw the scene
        // Update the scene
        window.clear();

        window.draw(spriteBackground);

        for (int i = 0; i < spriteClouds.size(); i++)
        {
            window.draw(spriteClouds[i].sprite);
        }

        window.draw(scoreText);

        window.draw(spriteBee);
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }
        window.draw(spriteTree);

        window.draw(player.sprite);
        window.draw(axe.sprite);
        window.draw(log.sprite);

        window.draw(timebarBack);
        window.draw(timebarFront);
        if (paused)
        {
            window.draw(gameText);
            window.draw(gravestone.sprite);
        }
        window.display();
    }
    return 0;
}

void updateBranches(int seed)
{
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPostition[j] = branchPostition[j - 1];
    }
    srand(int(time(0)) + seed);
    int r = rand() % 5;

    switch (r)
    {
    case 0:
        branchPostition[0] = side::LEFT;
        break;
    case 1:
        branchPostition[0] = side::RIGHT;
        break;
    default:
        branchPostition[0] = side::NONE;
    }
}