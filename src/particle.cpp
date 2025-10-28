#include "particle.hpp"
#include <cmath>

void Particle::update(float dt) noexcept {
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

Particle &ParticleManager::addObject(sf::Vector2f position,
                                     float radius) noexcept {
    Particle newParticle = Particle(position, radius);
    return objects.emplace_back(newParticle);
}

void ParticleManager::update() {
    float substep_dt = step_dt / sub_steps;
    for (int i = 0; i < sub_steps; ++i) {
        applyGravity();
        applyBoundary();
        checkCollisions();
        updateObjects(substep_dt);
    }
};

void ParticleManager::setBoundary(sf::Vector2f position,
                                  float radius) noexcept {
    boundary_center = position;
    boundary_radius = radius;
}

sf::Vector3f ParticleManager::getBoundary() const noexcept {
    return {boundary_center.x, boundary_center.y, boundary_radius};
}

void ParticleManager::setObjectVelocity(Particle &object,
                                        sf::Vector2f v) noexcept {
    object.setVelocity(v, getStepDt());
}

float ParticleManager::getStepDt() const noexcept {
    return step_dt / sub_steps;
}

void inline ParticleManager::applyGravity() noexcept {
    for (auto &obj : objects) {
        obj.accelerate((gravity));
    }
}

void inline ParticleManager::applyBoundary() noexcept {
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

void inline ParticleManager::checkCollisions() {
    int num_objects = objects.size();
    for (int i = 0; i < num_objects; ++i) {
        auto &obj_1 = objects[i];
        for (int j = 0; j < num_objects; j++) {
            if (i == j)
                continue;

            auto &obj_2 = objects[j];
            sf::Vector2f v = obj_1.position - obj_2.position;
            float dist = std::sqrt(v.x * v.x + v.y * v.y);
            float min_dist = obj_1.radius + obj_2.radius;

            if (dist < min_dist) {
                sf::Vector2f n = v / dist; // Normalize
                float total_mass =
                    obj_1.radius * obj_1.radius + obj_2.radius * obj_2.radius;
                float mass_ratio = (obj_1.radius * obj_1.radius) / total_mass;
                float delta = 0.5f * (min_dist - dist);

                obj_1.position += n * (1 - mass_ratio) * delta;
                obj_2.position -= n * mass_ratio * delta;
            }
        }
    }
}

void inline ParticleManager::updateObjects(float dt) noexcept {
    for (auto &obj : objects) {
        obj.update(dt);
    }
}

std::vector<Particle> &ParticleManager::getObjects() noexcept {
    return objects;
}

void ParticleManager::mousePull(sf::Vector2f pos) {
    for (auto &obj : objects) {
        sf::Vector2 dir = pos - obj.position;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        obj.accelerate(dir * std::max(0.0f, 10 * (120 - dist)));
    }
}

void ParticleManager::mousePush(sf::Vector2f pos) {
    for (auto &obj : objects) {
        sf::Vector2 dir = pos - obj.position;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        obj.accelerate(dir * std::min(0.0f, -10 * (120 - dist)));
    }
}

void ParticleManager::toggleGravityUp() noexcept { gravity = {0.0f, -1000.0f}; }
void ParticleManager::toggleGravityDown() noexcept {
    gravity = {0.0f, 1000.0f};
}
void ParticleManager::toggleGravityLeft() noexcept {
    gravity = {-1000.0f, 0.0f};
}
void ParticleManager::toggleGravityRight() noexcept {
    gravity = {1000.0f, 0.0f};
}
