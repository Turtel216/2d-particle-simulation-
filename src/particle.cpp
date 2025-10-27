#include "particle.hpp"

void Particle::update(float dt) {
    sf::Vector2f displacement = position - position_last;
    position_last = position;
    position = position + displacement + acceleration * (dt * dt);
    acceleration = {};
}

void Particle::setVelocity(sf::Vector2f v, float dt) {
    position_last = position - (v * dt);
}

void Particle::addVelocity(sf::Vector2f v, float dt) {
    position_last -= v * dt;
}

sf::Vector2f Particle::getVelocity() { return position - position_last; }
