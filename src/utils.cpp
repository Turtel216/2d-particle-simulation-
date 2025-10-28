#include "utils.hpp"
#include <cstdlib>

float getRandom() {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
