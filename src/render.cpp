#include "render.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include <string>

void Renderer::render(ParticleManager &manager) {
    // Draw boundary
    const sf::Vector3f boundary = manager.getBoundary();
    sf::CircleShape boundary_background{boundary.z};
    boundary_background.setOrigin(boundary.z, boundary.z);
    boundary_background.setFillColor(sf::Color::Black);
    boundary_background.setPosition(boundary.x, boundary.y);
    boundary_background.setPointCount(128);
    target.draw(boundary_background);

    // Draw Particles
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
