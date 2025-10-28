#include "render.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include <string>

void Renderer::render(ParticleManager &manager) {
    // Draw Particles
    sf::CircleShape circle{1.0f};
    circle.setPointCount(32);
    const auto &objects = manager.getObjects();
    for (const auto &obj : objects) {
        circle.setPosition(obj.position);
        circle.setScale(obj.radius, obj.radius);
        circle.setFillColor(obj.color);
        target.draw(circle);
    }
}
