#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <SFML/Graphics.hpp>
#include <random>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define HALF_WINDOW_HEIGHT WINDOW_HEIGHT / 2
#define HALF_WINDOW_WIDTH WINDOW_WIDTH / 2

extern std::mt19937 gen;

float getRandomNumber(int min, int max);

sf::Font loadFont();

class FPSCounter
{
private:
    float fps;
    sf::Text text;

public:
    FPSCounter(sf::Text text);
    void update(float deltaTime);
    void showFPS(sf::RenderWindow &window);
};

#endif // UTILITY_HPP