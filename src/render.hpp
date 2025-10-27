#ifndef RENDER_H_
#define RENDER_H_

#include "particle.hpp"
#include <SFML/Graphics.hpp>

class Renderer {
  public:
    Renderer(sf::RenderTarget &target_) : target{target_} {}

    void render(ParticleManager &manager);

  private:
    sf::RenderTarget &target;
};

#endif // RENDER_H_
