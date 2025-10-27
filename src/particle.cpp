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

void Particle::accelerate(sf::Vector2f a) { acceleration += a; }

Particle &ParticleManager::addObject(sf::Vector2f position, float radius) {
    Particle newParticle = Particle(position, radius);
    return objects.emplace_back(newParticle);
}

void ParticleManager::update() {
    applyGravity();
    updateObjects(step_dt);
};

void ParticleManager::applyGravity() {
    for (auto &obj : objects) {
        obj.accelerate((gravity));
    }
}

void ParticleManager::updateObjects(float dt) {
    for (auto &obj : objects) {
        obj.update(dt);
    }
}

std::vector<Particle> &ParticleManager::getObjects() { return objects; }
