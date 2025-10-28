#include "utils.hpp"
#include <cmath>
#include <cstdint>
#include <cstdlib>

float getRandom() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

sf::Color getColor(float t) {
    const float r = std::sin(t);
    const float g = std::sin(t + 0.33f * 2.0f * M_PI);
    const float b = std::sin(t + 0.66f * 2.0f * M_PI);
    return {static_cast<std::uint8_t>(255.0f * r * r),
            static_cast<std::uint8_t>(255.0f * g * g),
            static_cast<std::uint8_t>(255.0f * b * b)};
}
