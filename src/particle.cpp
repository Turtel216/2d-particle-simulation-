#include "particle.hpp"
#include "SFML/System/Vector2.hpp"
#include <cmath>

void Particle::update(const float dt) noexcept {
    sf::Vector2f displacement = position - position_last;
    position_last = position;
    position = position + displacement + acceleration * (dt * dt);
    acceleration = {};

    gridx = position.x / 15;
    gridy = position.y / 15;
}

void Particle::setVelocity(const sf::Vector2f &v, const float dt) noexcept {
    position_last = position - (v * dt);
}

void Particle::addVelocity(const sf::Vector2f &v, const float dt) noexcept {
    position_last -= v * dt;
}

sf::Vector2f Particle::getVelocity() noexcept {
    return position - position_last;
}

void Particle::accelerate(const sf::Vector2f &a) noexcept { acceleration += a; }

Particle &ParticleManager::addObject(const sf::Vector2f &position,
                                     const float radius) noexcept {
    int gridx = position.x / grid_size, gridy = position.x / grid_size;
    Particle newParticle =
        Particle(position, radius, gridx, gridy, objects.size());
    grid[gridx][gridy].push_back(objects.size());
    return objects.emplace_back(newParticle);
}

void ParticleManager::update() {
    float substep_dt = step_dt / sub_steps;
    for (int i = 0; i < sub_steps; ++i) {
        applyGravity();
        checkCollisions();
        applyBoundary();
        updateObjects(substep_dt);
    }
};

void ParticleManager::setObjectVelocity(Particle &object,
                                        const sf::Vector2f &v) noexcept {
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
        const float dampening = 0.75f;
        const sf::Vector2f pos = obj.position;
        sf::Vector2f new_pos = obj.position;
        sf::Vector2f vel = obj.getVelocity();
        sf::Vector2f dy = {vel.x * dampening, -vel.y};
        sf::Vector2f dx = {-vel.x, vel.y * dampening};

        // Bounce border vertical
        if (pos.x < obj.radius || pos.x + obj.radius > window_size) {
            if (pos.x < obj.radius)
                new_pos.x = obj.radius;
            if (pos.x + obj.radius > window_size)
                new_pos.x = window_size - obj.radius;

            obj.position = new_pos;
            obj.setVelocity(dx, 1.0);
        }

        // Bounce border horizontal
        if (pos.y < obj.radius || pos.y + obj.radius > window_size) {
            if (pos.y < obj.radius)
                new_pos.y = obj.radius;
            if (pos.y + obj.radius > window_size)
                new_pos.y = window_size - obj.radius;

            obj.position = new_pos;
            obj.setVelocity(dy, 1.0);
        }
    }
}

void inline ParticleManager::checkCollisions() noexcept {
    int num_objects = objects.size();
    for (Particle &obj_1 : objects) {
        for (int i : getCollisionParticles(obj_1.id)) {
            Particle &obj_2 = objects[i];
            sf::Vector2f v = obj_1.position - obj_2.position;
            float dist = sqrt(v.x * v.x + v.y * v.y);
            float min_dist = obj_1.radius + obj_2.radius;

            if (dist < min_dist) {
                sf::Vector2f n = v / dist; // Normalize
                float delta = 0.5f * (min_dist - dist);

                obj_1.position += n * 0.5f * delta;
                obj_2.position -= n * 0.5f * delta;
            }
        }
    }
}

std::vector<int>
ParticleManager::getCollisionParticles(int particleID) const noexcept {
    Particle p = objects[particleID];
    std::vector<int> res;
    for (int i = p.gridx - 1; i <= p.gridx + 1; ++i) {
        for (int j = p.gridy - 1; j <= p.gridy + 1; ++j) {
            if (i < 0 || j < 0 || i >= 56 || j >= 56)
                continue;
            for (int new_id : grid[i][j])
                if (new_id != p.id)
                    res.push_back(new_id);
        }
    }
    return res;
}

void inline ParticleManager::updateObjects(const float dt) noexcept {
    for (int i = 0; i < 100; ++i)
        for (int j = 0; j < 100; ++j)
            grid[i][j].clear();
    for (auto &obj : objects) {
        obj.update(dt);
        grid[obj.gridx][obj.gridy].push_back(obj.id);
    }
}

std::vector<Particle> &ParticleManager::getObjects() noexcept {
    return objects;
}

void ParticleManager::mousePull(const sf::Vector2f &pos) {
    for (auto &obj : objects) {
        sf::Vector2 dir = pos - obj.position;
        float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        obj.accelerate(dir * std::max(0.0f, 10 * (120 - dist)));
    }
}

void ParticleManager::mousePush(const sf::Vector2f &pos) {
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
