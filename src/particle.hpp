#ifndef PARTICAL_H_
#define PARTICAL_H_

#include <SFML/Graphics.hpp>
#include <vector>

/**
 * @file particle.hpp
 * @brief Simple particle and particle-system interfaces using SFML types.
 *
 * This header declares a lightweight Particle type using Verlet-like
 * integration (velocity is derived from the current and previous positions) and
 * a ParticleManager responsible for updating particles, applying global forces
 * (e.g., gravity), simple boundary constraints, and basic collision handling.
 */

/**
 * @class Particle
 * @brief Represents a single particle with position, radius, color, and
 * kinematics.
 *
 * Public members are intentionally simple for convenience in rendering and
 * quick prototyping. Velocity is implicit and derived from position and
 * position_last.
 */
class Particle {
  public:
    /**
     * @brief Rendering radius in pixels.
     *
     * Defaults to 10.0f for convenience. This affects collision approximation
     * (if enabled) and how large the particle appears when drawn.
     */
    float radius = 10.0f;

    /**
     * @brief Current particle position in pixels.
     */
    sf::Vector2f position;

    /**
     * @brief Previous particle position in pixels.
     *
     * Used to derive instantaneous velocity for Verlet-like integration:
     * velocity ≈ (position - position_last) / dt.
     */
    sf::Vector2f position_last;

    /**
     * @brief Current accumulated acceleration in pixels/s^2.
     *
     * Use accelerate() to add to this value. It is typically reset or
     * integrated during update().
     */
    sf::Vector2f acceleration;

    /**
     * @brief Particle color for rendering.
     *
     * Defaults to sf::Color::Cyan.
     */
    sf::Color color = sf::Color::Cyan;

    /**
     * @brief Default constructor.
     *
     * Leaves position vectors uninitialized; radius defaults to 10.0f and color
     * defaults to Cyan.
     */
    Particle() = default;

    /**
     * @brief Construct a particle at a given position and radius.
     *
     * Initializes position and position_last to the same value so that initial
     * velocity is zero. Acceleration is initialized to {10, 10} by default.
     *
     * @param position_ Initial position in pixels.
     * @param radius_   Rendering/collision radius in pixels.
     */
    Particle(const sf::Vector2f position_, const float radius_)
        : position{position_}, position_last{position_},
          acceleration{10.0f, 10.0f}, radius{radius_} {}

    /**
     * @brief Integrate particle state forward by dt seconds.
     *
     * Performs a Verlet-like step using position and position_last, applying
     * the current acceleration. Implementations typically:
     * - compute velocity from position - position_last,
     * - update position by velocity and acceleration,
     * - store current position in position_last for the next step,
     * - clear or damp acceleration as appropriate.
     *
     * @param dt Time step in seconds. Must be positive.
     */
    void update(const float dt) noexcept;

    /**
     * @brief Add a velocity to the particle for this step.
     *
     * In Verlet-style integration, adding velocity can be achieved by shifting
     * position_last relative to position:
     * position_last = position - (existing_velocity + v) * dt.
     *
     * @param v  Velocity to add (pixels/s).
     * @param dt Time step in seconds used to convert velocity to a position
     * delta.
     */
    void addVelocity(const sf::Vector2f &v, const float dt) noexcept;

    /**
     * @brief Set the particle's instantaneous velocity.
     *
     * In Verlet-style integration, this typically adjusts position_last such
     * that (position - position_last) / dt == v.
     *
     * @param v  Desired velocity (pixels/s).
     * @param dt Time step in seconds used to convert velocity to a position
     * delta.
     */
    void setVelocity(const sf::Vector2f &v, const float dt) noexcept;

    /**
     * @brief Add to the particle's acceleration accumulator.
     *
     * @param a Acceleration to add (pixels/s^2).
     */
    void accelerate(const sf::Vector2f &a) noexcept;

    /**
     * @brief Get the particle's instantaneous velocity estimate.
     *
     * Derived as (position - position_last) / dt in the integration step; this
     * accessor may provide the last known or current-step estimate depending on
     * implementation.
     *
     * @return sf::Vector2f Approximated velocity in pixels/s.
     */
    sf::Vector2f getVelocity() noexcept;
};

/**
 * @class ParticleManager
 * @brief Manages a collection of particles, global forces, boundaries, and
 * updates.
 *
 * Provides helper operations to:
 * - Add particles,
 * - Apply mouse-based push/pull interactions,
 * - Update all particles with fixed sub-steps,
 * - Constrain particles within a circular boundary,
 * - Toggle gravity direction.
 */
class ParticleManager {
  public:
    /**
     * @brief Default constructor initializes reasonable simulation defaults.
     *
     * Defaults:
     * - gravity = {0, 1000} pixels/s^2 (downward),
     * - boundary_center = {420, 420} pixels,
     * - window_size = 840 pixels (used with boundary),
     * - boundary_radius = 100 pixels,
     * - step_dt = 1/60 seconds,
     * - sub_steps = 8 (for stability via smaller internal time steps).
     */
    ParticleManager() = default;

    /**
     * @brief Apply an attractive mouse force toward the given position.
     *
     * The exact force model is implementation-defined (e.g., inverse distance,
     * capped magnitude). Typically used for interactive demos.
     *
     * @param pos Mouse position in pixels (SFML coordinates).
     */
    void mousePull(const sf::Vector2f &pos);

    /**
     * @brief Apply a repulsive mouse force away from the given position.
     *
     * The exact force model is implementation-defined (e.g., inverse distance,
     * capped magnitude). Typically used for interactive demos.
     *
     * @param pos Mouse position in pixels (SFML coordinates).
     */
    void mousePush(const sf::Vector2f &pos);

    /**
     * @brief Create and add a new particle to the system.
     *
     * @param position Initial position in pixels.
     * @param radius   Particle radius in pixels.
     * @return Particle& Reference to the newly created particle.
     *
     * @note The reference remains valid as long as the underlying vector does
     *       not reallocate. Retain with care if you plan to add many particles.
     */
    Particle &addObject(const sf::Vector2f &position,
                        const float radius) noexcept;

    /**
     * @brief Access all managed particles.
     *
     * @return std::vector<Particle>& Mutable reference to the particle array.
     */
    std::vector<Particle> &getObjects() noexcept;

    /**
     * @brief Advance the simulation by one frame.
     *
     * Splits the nominal step (step_dt) into a number of sub-steps (sub_steps)
     * for improved stability, applying gravity, boundary constraints, collision
     * checks, and particle updates each sub-step.
     */
    void update();

    /**
     * @brief Define a circular boundary constraint for particles.
     *
     * Particles are constrained to remain within the circle defined by center
     * and radius; constraint response is implementation-defined (e.g.,
     * clamping, reflection).
     *
     * @param position Circle center in pixels.
     * @param radius   Circle radius in pixels.
     */
    void setBoundary(const sf::Vector2f &position, const float radius) noexcept;

    /**
     * @brief Query the current circular boundary as (x, y, r).
     *
     * @return sf::Vector3f A vector where (x, y) is the center in pixels and z
     * is the radius in pixels.
     */
    sf::Vector3f getBoundary() const noexcept;

    /**
     * @brief Set the instantaneous velocity of a specific particle.
     *
     * This is a convenience wrapper around the particle's own setVelocity(),
     * typically adjusting position_last to match the desired velocity under
     * Verlet-style integration.
     *
     * @param object Particle to modify.
     * @param v      Desired velocity (pixels/s).
     */
    void setObjectVelocity(Particle &object, const sf::Vector2f &v) noexcept;

    /**
     * @brief Get the fixed time step used for a full frame update.
     *
     * @return float Fixed time step in seconds (e.g., 1/60).
     */
    float getStepDt() const noexcept;

    /**
     * @brief Toggle gravity orientation to act upward (negative Y).
     *
     * The exact toggle semantics are implementation-defined. Intended for
     * interactive control of gravity direction.
     */
    void toggleGravityUp() noexcept;

    /**
     * @brief Toggle gravity orientation to act downward (positive Y).
     *
     * The exact toggle semantics are implementation-defined. Intended for
     * interactive control of gravity direction.
     */
    void toggleGravityDown() noexcept;

    /**
     * @brief Toggle gravity orientation to act leftward (negative X).
     *
     * The exact toggle semantics are implementation-defined. Intended for
     * interactive control of gravity direction.
     */
    void toggleGravityLeft() noexcept;

    /**
     * @brief Toggle gravity orientation to act rightward (positive X).
     *
     * The exact toggle semantics are implementation-defined. Intended for
     * interactive control of gravity direction.
     */
    void toggleGravityRight() noexcept;

  private:
    /**
     * @brief Container of all managed particles.
     */
    std::vector<Particle> objects;

    /**
     * @brief Global gravity vector in pixels/s^2.
     *
     * Defaults to downward gravity {0, 1000}.
     */
    sf::Vector2f gravity = {0.0f, 1000.0f};

    /**
     * @brief Nominal window size in pixels (used for clamping or scaling
     * behavior).
     */
    float window_size = 840.0f;

    /**
     * @brief Radius of the circular boundary in pixels.
     */
    float boundary_radius = 100.0f;

    /**
     * @brief Fixed frame time step in seconds.
     *
     * The update() method typically uses this as the total per-frame dt, which
     * is internally subdivided into sub-steps for stability.
     */
    float step_dt = 1.0f / 60;

    /**
     * @brief Number of physics sub-steps per frame.
     *
     * Higher values can improve stability at the cost of CPU time.
     */
    float sub_steps = 8;

    /**
     * @brief Apply global gravity to all particles.
     *
     * Adds the gravity vector to each particle's acceleration.
     */
    void inline applyGravity() noexcept;

    /**
     * @brief Constrain particles to remain within the circular boundary.
     *
     * Implementation may clamp positions and/or adjust velocities to keep
     * particles inside the defined circle.
     */
    void inline applyBoundary() noexcept;

    /**
     * @brief Resolve inter-particle collisions.
     *
     * Typically uses simple circle overlap resolution based on particle radii.
     * Behavior is implementation-specific (e.g., positional correction only).
     */
    void inline checkCollisions();

    /**
     * @brief Update all particles by a sub-step dt.
     *
     * Performs per-particle integration (e.g., Verlet), then clears or applies
     * damping to acceleration as appropriate.
     *
     * @param dt Sub-step time delta in seconds.
     */
    void inline updateObjects(const float dt) noexcept;
};

#endif // PARTICAL_H_
