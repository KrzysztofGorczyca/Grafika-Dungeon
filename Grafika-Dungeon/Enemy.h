#ifndef ENEMY_H
#define ENEMY_H

#include "libs.h"
#include "Camera.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define ModelPath "Assets/Skeleton/skeleton.obj"

/**
 * @brief Class representing the hand of an enemy, used for handling hand-specific animations and transformations.
 */
class enemyHand {
public:
    glm::vec3 position; ///< The position of the enemy's hand
    glm::quat rotation; ///< The rotation of the enemy's hand
    float rotationAngle = 0.0f; ///< The angle of rotation for the hand

    /**
     * @brief Constructs an enemyHand object.
     *
     * @param pos Initial position of the hand.
     */
    enemyHand(const glm::vec3& pos)
        : position(pos), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
    {}

    /**
     * @brief Sets the rotation of the hand.
     *
     * @param angle The angle to rotate the hand by.
     */
    void SetRotation(float angle) {
        rotation = glm::angleAxis(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
    }
};

/**
 * @brief Class representing an enemy in the game, with properties and methods for controlling the enemy's behavior and state.
 */
class Enemy {
public:
    bool isAnimating = false; ///< Indicates if the enemy is currently animating
    float animationTime = 0.0f; ///< The current time of the animation
    float attackDuration = 1.5f; ///< Duration of the attack animation
    float returnDuration = 1.5f; ///< Duration of the return animation
    bool isReturning = false; ///< Indicates if the enemy is in the return phase of the animation
    bool onceDone = false;
    bool damagePlayer = false; ///< Indicates if the enemy can damage the player
    bool animationFinished = false; ///< Indicates if the animation has finished
    bool inAttackMode = false; ///< Indicates if the enemy is in attack mode
    glm::vec3 Position; ///< The position of the enemy
    glm::quat Rotation; ///< The rotation of the enemy
    float Speed; ///< The movement speed of the enemy
    float Health = 100.f; ///< The current health of the enemy
    float Damage = 10; ///< The damage the enemy can inflict
    float MaxHealth = 100.0f; ///< The maximum health of the enemy
    enemyHand hand; ///< The hand of the enemy
    bool attacked = false;
    bool canDamage = true;
    bool inRange = false;
    bool dealDamage = false;

    /**
     * @brief Constructs an Enemy object.
     *
     * @param position Initial position of the enemy.
     */
    Enemy(glm::vec3 position)
        : Position(position)
        , Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        , Speed(1.0f)
        , hand(position + glm::vec3(0.088918f, 0.014702f, 0.77669f))
    {}

    /**
     * @brief Destructor for the Enemy class.
     */
    ~Enemy() {}

    /**
     * @brief Plays the hand animation for the enemy.
     *
     * @param deltaTime The time elapsed between the current and previous frame.
     */
    void playHandAnimation(float deltaTime) {
        if (inAttackMode) {
            animationTime += deltaTime;
            float t;

            if (!isReturning) {
                t = animationTime / attackDuration;
                float startX = 0.0f;  // Starting X offset
                float endX = -70.0f;   // Ending X offset
                float currentX = glm::mix(startX, endX, t);
                hand.rotationAngle = currentX;

                if (animationTime >= attackDuration) {
                    damagePlayer = true;
                    animationTime = 0.0f;
                    isReturning = true;
                    attacked = true;
                }
            }
            else {
                t = animationTime / returnDuration;
                float startX = -70.0f;  // Starting X offset
                float endX = 0.0f;     // Ending X offset
                float currentX = glm::mix(startX, endX, t);
                hand.rotationAngle = currentX;

                if (animationTime >= returnDuration) {
                    isAnimating = false;
                    isReturning = false;
                    animationTime = 0.0f;
                    damagePlayer = false;
                    hand.rotationAngle = 0.0f;
                    animationFinished = true;
                    inAttackMode = false;
                }
            }
        }
    }

    /**
     * @brief Updates the enemy's state, including movement and other logic.
     *
     * @param deltaTime The time elapsed between the current and previous frame.
     * @param camera The camera object, used for determining the player's position.
     */
    void Update(float deltaTime, const Camera& camera) {
        if (!isDead()) {
            glm::vec3 direction = camera.getCameraPosition() - Position;
            direction.y = 0.0f;
            direction = glm::normalize(direction);

            if (isDead()) {
                canDamage = false;
                std::cout << "Enemy is dead" << std::endl;
            }

            float distance = glm::distance(glm::vec2(Position.x, Position.z), glm::vec2(camera.getCameraPosition().x, camera.getCameraPosition().z));
            if (distance < 5.0f && distance > 1.0f) {
                Position += direction * Speed * deltaTime;
                rotateEnemy(direction);
                if (direction.x < -69.0f) {
                    dealDamage = true;
                }
            }
            else if (distance < 1.0f) {
                rotateEnemy(direction);
            }

            Position.y = 0.0f;
        }
    }

    /**
     * @brief Modifies the enemy's health.
     *
     * @param amount The amount to modify the health by.
     */
    void modifyHealth(float amount) {
        Health += amount;
        if (Health < 0.0f) Health = 0.0f;
        if (Health > MaxHealth) Health = MaxHealth;
    }

    /**
     * @brief Checks if the enemy is dead.
     *
     * @return True if the enemy's health is 0 or less, false otherwise.
     */
    bool isDead() {
        if (Health <= 0.0f) {
            Position = glm::vec3(0.0f, 0.0f, 100.0f);
            return true;
        }
        return false;
    }

    /**
     * @brief Gets the position of the enemy.
     *
     * @return The current position of the enemy.
     */
    inline glm::vec3 GetPosition() const { return Position; }

    /**
     * @brief Gets the health of the enemy.
     *
     * @return The current health of the enemy.
     */
    inline float GetHealth() const { return Health; }

    /**
     * @brief Updates the position of the enemy.
     *
     * @param position The new position to set.
     */
    void UpdatePosition(glm::vec3 position) {
        Position = position;
    }

private:
    /**
     * @brief Rotates the enemy to face a given direction.
     *
     * @param direction The direction to face.
     */
    void rotateEnemy(glm::vec3 direction) {
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);  // Assume initial direction is along Z axis
        glm::vec2 dir2D = glm::normalize(glm::vec2(direction.x, direction.z));
        glm::vec2 forward2D = glm::normalize(glm::vec2(forward.x, forward.z));
        float angle = atan2(dir2D.y, dir2D.x) - atan2(forward2D.y, forward2D.x);

        glm::quat rotationQuat = glm::angleAxis(-angle, glm::vec3(0.0f, 1.0f, 0.0f));
        Rotation = rotationQuat;
    }
};

#endif // ENEMY_H
