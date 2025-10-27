#ifndef PARTICAL_H_
#define PARTICAL_H_

#include <SFML/Graphics.hpp>

class Particle {
  public:
    Particle() = default;

    Particle(sf::Vector2f position_, float radius_)
        : position{position_}, position_last{position_},
          acceleration{10.0f, 10.0f}, radius{radius_} {}

    void update(float dt);
    void addVelocity(sf::Vector2f v, float dt);
    void setVelocity(sf::Vector2f v, float dt);
    void accelerate(sf::Vector2f a);
    sf::Vector2f getVelocity();

  private:
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    float radius = 10.0f;
};

#endif // PARTICAL_H_
