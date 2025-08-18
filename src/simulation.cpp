#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <random>
#include <time.h>
#include <cmath>
#include "utility.cpp"

#define BALL_COUNT 700 // limit for around 60 FPS

class Particle
{
    float mass;
    sf::Vector2f velocity = {
        120.f, 120.f};
    float acceleration;
    int isSpawned = 0;

    sf::CircleShape shape;

public:
    Particle()
    {
        this->shape.setFillColor(sf::Color::White);
        this->mass = getRandomNumber(1.f, 10.f);
        float radius = sqrt(this->mass) * 2.f;
        this->shape.setOrigin({radius, radius});
        this->shape.setRadius(radius);
    }

    float getMass()
    {
        return this->mass;
    }

    sf::Vector2f getVelocity()
    {
        return this->velocity;
    }

    float radius()
    {
        return this->shape.getRadius();
    }

    sf::CircleShape spawn(float deltaTime)
    {
        if (!this->isSpawned)
        {
            float x = getRandomNumber(0, WINDOW_WIDTH - this->radius() * 2);
            float y = getRandomNumber(0, WINDOW_HEIGHT - this->radius() * 2);
            this->shape.setPosition({x, y});
            this->isSpawned = 1;
            return this->shape;
        }

        return this->move(deltaTime);
    }

    sf::CircleShape move(float deltaTime)
    {
        // S = S₀ + v ⋅ t
        float x = this->shape.getPosition().x + this->velocity.x * deltaTime;

        if (x + this->radius() >= WINDOW_WIDTH || x - this->radius() <= 0)
        {
            this->velocity.x *= -1;
        }

        float y = this->shape.getPosition().y + this->velocity.y * deltaTime;

        if (y + this->radius() >= WINDOW_HEIGHT || y - this->radius() <= 0)
        {
            this->velocity.y *= -1;
        }

        this->shape.setPosition({x, y});
        return this->shape;
    }

    sf::Vector2f getCenterPoint()
    {
        sf::Vector2f points =
            {this->shape.getPosition().x,
             this->shape.getPosition().y};

        return points;
    }

    int handleCollision(Particle *other)
    {
        float xDistance = abs(this->getCenterPoint().x - other->getCenterPoint().x);
        float yDistance = abs(this->getCenterPoint().y - other->getCenterPoint().y);

        // a² + b² = c²
        float h = (xDistance * xDistance) + (yDistance * yDistance);
        float distance = sqrt(h);

        float sumOfRadius = this->radius() + other->radius();

        if (distance <= sumOfRadius)
        {
            this->handleImpact(other, distance);
            return 1;
        }

        return 0;
    }

    void handleImpact(Particle *other, float distance)
    {
        float idealLineOfImpact = this->radius() + other->radius();
        sf::Vector2f centerPoint = this->getCenterPoint();
        sf::Vector2f otherCenterPoint = other->getCenterPoint();

        if (distance < idealLineOfImpact)
        {
            std::cout << "Overlaping!" << "\n";
            float overlap = (idealLineOfImpact - distance) / 2;
            sf::Vector2f direction;
            if (distance != 0.f)
            {
                direction = (other->getCenterPoint() - this->getCenterPoint()) / distance;
            }
            else
            {
                direction = sf::Vector2f(1.f, 0.f);
            }
            sf::Vector2f normalizedDirection = direction;
            this->shape.setPosition(this->getCenterPoint() - normalizedDirection * overlap);
            other->shape.setPosition(other->getCenterPoint() + normalizedDirection * overlap);
        }

        // Particle 1
        float massFactor = 2.f * other->getMass() / (this->getMass() + other->getMass());
        sf::Vector2f relativeVelocity = other->getVelocity() - this->getVelocity();
        sf::Vector2f lineOfImpact = other->getCenterPoint() - this->getCenterPoint();

        float scalarProjectionNumerator = (relativeVelocity.x * lineOfImpact.x) + (relativeVelocity.y * lineOfImpact.y);
        float scalarProjectionDenominator = (lineOfImpact.x * lineOfImpact.x) + (lineOfImpact.y * lineOfImpact.y);

        float scalarProjection = scalarProjectionNumerator / scalarProjectionDenominator;
        sf::Vector2f v1Prime = this->getVelocity() + massFactor * scalarProjection * lineOfImpact;

        this->velocity = v1Prime;

        // Particle 1
        float massFactor2 = 2.f * this->getMass() / (this->getMass() + other->getMass());
        sf::Vector2f relativeVelocity2 = this->getVelocity() - other->getVelocity();
        sf::Vector2f lineOfImpact2 = this->getCenterPoint() - other->getCenterPoint();

        float scalarProjectionNumerator2 = (relativeVelocity2.x * lineOfImpact2.x) + (relativeVelocity2.y * lineOfImpact2.y);
        float scalarProjectionDenominator2 = (lineOfImpact2.x * lineOfImpact2.x) + (lineOfImpact2.y * lineOfImpact2.y);

        float scalarProjection2 = scalarProjectionNumerator2 / scalarProjectionDenominator2;
        sf::Vector2f v2Prime = other->getVelocity() + massFactor2 * scalarProjection2 * lineOfImpact2;

        other->velocity = v2Prime;
    }
};

class Simulation
{
    sf::RenderWindow &window;
    std::vector<Particle> particles;

public:
    Simulation(sf::RenderWindow &window) : window(window)
    {
        for (int i = 0; i < BALL_COUNT; i++)
        {
            Particle p;
            this->particles.push_back(p);
        }
    }

    void update(float deltaTime)
    {
        for (int i = 0; i < this->particles.size(); i++)
        {
            this->window.draw(this->particles[i].spawn(deltaTime));

            for (int j = i + 1; j < this->particles.size(); j++)
            {
                this->particles[i].handleCollision(&this->particles[j]);
            }
        }
    }
};