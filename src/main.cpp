#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/ContextSettings.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "particle.hpp"
#include "render.hpp"
#include "utils.hpp"
#include <cmath>

int main(int argc, char *argv[]) {
    constexpr int32_t window_width = 840;
    constexpr int32_t window_height = 840;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height),
                            "2D Particle Simulation", sf::Style::Default,
                            settings);

    sf::Font arialFont;
    arialFont.loadFromFile("./assets/Arial Unicode.ttf");

    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);
    Renderer renderer{window};

    ParticleManager manager;
    manager.setBoundary({window_width / 2.0f, window_height / 2.0f},
                        (window_width - 20.0f) / 2);

    const int max_objects = 1000; // Maximum number of particles being spawned
    const float spawn_delay = 0.05f;
    const sf::Vector2f spawn_position = {420.0f,
                                         200.0f}; // Particle spawn point
    const float min_radius = 7.0f;                // Minimum particle radius
    const float max_radius = 12.0f;               // Maximum particle radius
    const float spawn_velocity =
        200.0f; // Initiale particle velocity after spawn
    const float max_angle = M_PI * 0.5f; // Maximum spawn angle

    // Clock for tracking spawn intervals, spawn angle and fps
    sf::Clock spawn_clock, timer, fps_timer;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                sf::Keyboard::isKeyPressed(
                    sf::Keyboard::Escape)) { // Terminate program
                window.close();
            }
        }

        // Move gravity on key press
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            manager.toggleGravityUp();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            manager.toggleGravityDown();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            manager.toggleGravityLeft();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            manager.toggleGravityRight();

        // Spaen Particles
        if (manager.getObjects().size() < max_objects &&
            spawn_clock.getElapsedTime().asSeconds() >= spawn_delay) {
            float t = timer.getElapsedTime().asSeconds();
            float radius = min_radius + (max_radius - min_radius) * getRandom();
            auto &object = manager.addObject(spawn_position, radius);

            float angle = M_PI * 0.5f + max_angle * std::sin(3 * t);
            object.color = getColor(t);

            manager.setObjectVelocity(
                object, spawn_velocity *
                            sf::Vector2f(std::cos(angle), std::sin(angle)));
            spawn_clock.restart();
        }

        // Mouuse pull
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            float ratio = 840.0f / window.getSize().x;
            sf::Vector2f pos =
                static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) *
                ratio;
            manager.mousePull(pos);
        }

        // Mouse Push
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            float ratio = 840.0f / window.getSize().x;
            sf::Vector2f pos =
                static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) *
                ratio;
            manager.mousePush(pos);
        }

        fps_timer.restart();

        manager.update();

        float ms = 1.0 * fps_timer.getElapsedTime().asMicroseconds() / 100;

        window.clear(sf::Color::White);
        renderer.render(manager);

        ms = 1.0 * fps_timer.getElapsedTime().asMicroseconds() / 1000;

        // Draw perfomance info
        sf::Text number;
        number.setFont(arialFont);
        number.setString(std::to_string(ms) + "ms, " +
                         std::to_string(manager.getObjects().size()) +
                         " particles");
        number.setCharacterSize(24);
        number.setFillColor(sf::Color::Black);
        window.draw(number);

        window.display();
    }

    return 0;
}
