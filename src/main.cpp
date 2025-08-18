#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <random>
#include <time.h>
#include <cmath>
#include "utility.hpp"
#include "simulation.cpp"

using namespace std;

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Elastic Collision");
    window.setFramerateLimit(144);

    sf::Font font = loadFont();
    sf::Text text(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    sf::Clock clock;
    float deltaTime;

    Simulation simulation(window);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        deltaTime = clock.restart().asSeconds();
        FPSCounter fpsCounter(text);
        fpsCounter.update(deltaTime);

        window.clear();

        simulation.start(deltaTime);
        fpsCounter.showFPS(window);

        window.display();
    }
}
