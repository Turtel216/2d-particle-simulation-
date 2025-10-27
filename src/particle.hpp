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

    void update(float dt) noexcept;
    void addVelocity(sf::Vector2f v, float dt) noexcept;
    void setVelocity(sf::Vector2f v, float dt) noexcept;
    void accelerate(sf::Vector2f a) noexcept;
    sf::Vector2f getVelocity() noexcept;
};

class ParticleManager {
  public:
    ParticleManager() = default;

    Particle &addObject(sf::Vector2f position, float radius) noexcept;
    std::vector<Particle> &getObjects() noexcept;
    void update();
    void setBoundary(sf::Vector2f position, float radius) noexcept;
    sf::Vector3f getBoundary() const noexcept;

  private:
    std::vector<Particle> objects;
    sf::Vector2f gravity = {0.0f, 1000.0f};
    sf::Vector2f boundary_center = {420.0f, 420.0f};
    float boundary_radius = 100.0f;
    float step_dt = 1.0f / 60;

    void inline applyGravity() noexcept;
    void inline applyBoundary() noexcept;
    void inline updateObjects(float dt) noexcept;
};

#endif // PARTICAL_H_
