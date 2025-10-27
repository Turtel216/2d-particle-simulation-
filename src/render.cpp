#include "render.hpp"
#include <string>

void Renderer::render() {
    target.clear(sf::Color::Black);
    sf::CircleShape circle{1.0f};
    circle.setPointCount(32);
    circle.setPosition({420.0f, 420.0f});
    circle.setScale(10.0f, 10.0f);
    circle.setFillColor(sf::Color::Blue);
    target.draw(circle);
}
