#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/ContextSettings.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "particle.hpp"
#include "render.hpp"

int main(int argc, char *argv[]) {
    constexpr int32_t window_width = 840;
    constexpr int32_t window_height = 840;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height),
                            "Verlet", sf::Style::Default, settings);

    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);
    Renderer renderer{window};

    ParticleManager manager;
    manager.setBoundary({window_width / 2.0f, window_height / 2.0f},
                        (window_width - 20.0f) / 2);

    const int max_objects = 100;
    const float spawn_delay = 0.05f;
    const sf::Vector2f spawn_position = {420.0f, 200.0f};

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                sf::Keyboard::isKeyPressed(
                    sf::Keyboard::Escape)) { // Terminate program
                window.close();
            }
        }
        // Spaen Particles
        if (manager.getObjects().size() < max_objects &&
            clock.getElapsedTime().asSeconds() >= spawn_delay) {
            clock.restart();
            auto &object = manager.addObject(spawn_position, 10.0f);
            manager.setObjectVelocity(object, {500.0f, 50.0f});
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

        manager.update();
        window.clear(sf::Color::White);
        renderer.render(manager);
        window.display();
    }

    return 0;
}
