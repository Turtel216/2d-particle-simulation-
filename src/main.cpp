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
    auto &object = manager.addObject({420.0f, 100.0f}, 10.0f);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                sf::Keyboard::isKeyPressed(
                    sf::Keyboard::Escape)) { // Terminate program
                window.close();
            }
        }
        manager.update();
        window.clear(sf::Color::White);
        renderer.render(manager);
        window.display();
    }

    return 0;
}
