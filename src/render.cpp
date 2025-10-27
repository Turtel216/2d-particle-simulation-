#include "render.hpp"
#include <string>

void Renderer::render(ParticleManager &manager) {
    sf::CircleShape circle{1.0f};
    circle.setPointCount(32);
    const auto &objects = manager.getObjects();
    for (const auto &obj : objects) {
        circle.setPosition(obj.position);
        circle.setScale(obj.radius, obj.radius);
        circle.setFillColor(sf::Color::Cyan);
        target.draw(circle);
    }
}
