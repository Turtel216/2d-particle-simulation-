#include "particle.hpp"
#include <cmath>

void Particle::update(float dt) {
    sf::Vector2f displacement = position - position_last;
    position_last = position;
    position = position + displacement + acceleration * (dt * dt);
    acceleration = {};
}

void Particle::setVelocity(sf::Vector2f v, float dt) noexcept {
    position_last = position - (v * dt);
}

void Particle::addVelocity(sf::Vector2f v, float dt) noexcept {
    position_last -= v * dt;
}

sf::Vector2f Particle::getVelocity() noexcept {
    return position - position_last;
}

void Particle::accelerate(sf::Vector2f a) noexcept { acceleration += a; }

Particle &ParticleManager::addObject(sf::Vector2f position, float radius) {
    Particle newParticle = Particle(position, radius);
    return objects.emplace_back(newParticle);
}

void ParticleManager::update() {
    applyGravity();
    applyBoundary();
    updateObjects(step_dt);
};

void ParticleManager::setBoundary(sf::Vector2f position,
                                  float radius) noexcept {
    boundary_center = position;
    boundary_radius = radius;
}

sf::Vector3f ParticleManager::getBoundary() const noexcept {
    return {boundary_center.x, boundary_center.y, boundary_radius};
}

void inline ParticleManager::applyGravity() {
    for (auto &obj : objects) {
        obj.accelerate((gravity));
    }
}

void inline ParticleManager::applyBoundary() {
    for (auto &obj : objects) {
        const sf::Vector2f r = boundary_center - obj.position;
        const float dist = std::sqrt(r.x * r.x + r.y * r.y);

        if (dist > boundary_radius - obj.radius) {
            const sf::Vector2f n = r / dist;
            const sf::Vector2f perp = {-n.y, n.x};
            const sf::Vector2f vel = obj.getVelocity();

            obj.position = boundary_center - n * (boundary_radius - obj.radius);
            obj.setVelocity(
                2.0f * (vel.x * perp.x + vel.y * perp.y) * perp - vel, 1.0f);
        }
    }
}

void ParticleManager::updateObjects(float dt) {
    for (auto &obj : objects) {
        obj.update(dt);
    }
}

std::vector<Particle> &ParticleManager::getObjects() noexcept {
    return objects;
}
