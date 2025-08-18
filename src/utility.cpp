#include "utility.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <time.h>
#include <cmath>

std::mt19937 gen(time(NULL)); // Definição única

float getRandomNumber(int min, int max)
{
    std::uniform_real_distribution<float> distrib(min, max);
    float value = distrib(gen);
    return value;
}

sf::Font loadFont()
{
    sf::Font font;
    if (!font.openFromFile("src/assets/roboto.ttf"))
    {
        std::cout << "Error importing font" << "\n";
        return font;
    }

    return font;
}

FPSCounter::FPSCounter(sf::Text text) : fps(0.0f), text(text)
{
    this->text.setFillColor(sf::Color::White);
    this->text.setPosition({10.f, 5.f});
}

void FPSCounter::update(float deltaTime)
{
    if (deltaTime > 0)
        fps = 1.0f / deltaTime;

    std::ostringstream ss;
    ss << "FPS: " << std::fixed << std::setprecision(1) << fps;
    this->text.setString(ss.str());
}

void FPSCounter::showFPS(sf::RenderWindow &window)
{
    window.draw(text);
}
