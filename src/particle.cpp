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
    int num_cells = window_size / grid_size;
    int dx[] = {1, 1, 0, 0, -1};
    int dy[] = {0, 1, 0, 1, 1};
    for (int i = 0; i < num_cells; ++i) {
        for (int j = 0; j < num_cells; ++j) {
            if (!grid[i][j].size())
                continue;
            for (int k = 0; k < 5; ++k) {
                int nx = i + dx[k], ny = j + dy[k];
                if (nx < 0 || ny < 0 || nx >= num_cells || ny >= num_cells)
                    continue;
                collideCells(i, j, nx, ny);
            }
        }
    }
}

void inline ParticleManager::collideCells(int x1, int y1, int x2,
                                          int y2) noexcept {
    for (int id_1 : grid[x1][y1]) {
        Particle &obj_1 = objects[id_1];
        for (int id_2 : grid[x2][y2]) {
            if (id_1 == id_2)
                continue;

            Particle &obj_2 = objects[id_2];
            sf::Vector2f v = obj_1.position - obj_2.position;
            float dist = sqrt(v.x * v.x + v.y * v.y);
            float min_dist = obj_1.radius + obj_2.radius;

            if (dist < min_dist) {
                sf::Vector2f n = v / dist; // Normalize
                // Big thing moves less
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

void inline ParticleManager::updateObjects(const float dt) noexcept {
    for (auto &obj : objects) {
        int cur_gridx = obj.gridx, cur_gridy = obj.gridy;
        obj.update(dt);
        obj.gridx = obj.position.x / grid_size;
        obj.gridy = obj.position.y / grid_size;

        if (cur_gridx != obj.gridx || cur_gridy != obj.gridy) {
            auto pos = find(grid[cur_gridx][cur_gridy].begin(),
                            grid[cur_gridx][cur_gridy].end(), obj.id);
            grid[cur_gridx][cur_gridy].erase(pos);
            grid[obj.gridx][obj.gridy].push_back(obj.id);
        }
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
