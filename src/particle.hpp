#ifndef PARTICAL_H_
#define PARTICAL_H_

#include <SFML/Graphics.hpp>
#include <vector>

class Particle {
  public:
    float radius = 10.0f;
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;

    Particle() = default;

    Particle(sf::Vector2f position_, float radius_)
        : position{position_}, position_last{position_},
          acceleration{10.0f, 10.0f}, radius{radius_} {}

    void update(float dt);
    void addVelocity(sf::Vector2f v, float dt);
    void setVelocity(sf::Vector2f v, float dt);
    void accelerate(sf::Vector2f a);
    sf::Vector2f getVelocity();
};

class ParticleManager {
  public:
    ParticleManager() = default;

    Particle &addObject(sf::Vector2f position, float radius);
    std::vector<Particle> &getObjects();
    void update();

  private:
    std::vector<Particle> objects;
    sf::Vector2f gravity = {0.0f, 1000.0f};
    float step_dt = 1.0f / 60;

    void applyGravity();
    void updateObjects(float dt);
};

#endif // PARTICAL_H_
