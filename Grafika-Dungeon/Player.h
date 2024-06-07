#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <cmath>
#include <chrono>
#include "libs.h"
#include "Enemy.h"

/**
 * @class Player
 * @brief Class representing the player character in the game.
 */
class Player
{
public:
    glm::vec3 Position;     ///< Player's position on the map.
    float Health = 100.00f; ///< Player's health.
    float MaxHealth = 100.00f; ///< Player's maximum health.
    float Damage = 25.0f; ///< Damage dealt by the player.
    float Range = 1.50f; ///< Player's attack range.
    float Regeneration = 5.0f; ///< Player's health regeneration rate.
    int regenTime = 0; ///< Time counter for health regeneration.
    glm::vec3 swordOffset; ///< Offset for the sword's position.
    glm::quat swordRotation; ///< Default rotation of the sword.
    bool isAnimating = false; ///< Flag to check if the player is animating.
    bool wasAttacked = false; ///< Flag to check if the player was attacked.
    float animationTime = 0.0f; ///< Timer for the sword animation.
    float attackDuration = 0.4f; ///< Duration of the attack animation.
    float returnDuration = 0.8f; ///< Duration of the return animation.
    bool isReturning = false; ///< Flag to check if the sword is returning.

    bool holdingE = false; ///< Flag to check if the player is holding the 'E' key.
    bool playAttack = false; ///< Flag to check if the attack sound should play.
    bool playReturn = false; ///< Flag to check if the return sound should play.
    bool playDamageSound = false; ///< Flag to check if the damage sound should play.

    /**
     * @brief Constructor for the Player class.
     * @param position Initial position of the player.
     */
    Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f))
        : Position(position), lastHealthUpdateTime(std::chrono::high_resolution_clock::now()),
        swordOffset(glm::vec3(0.23f, -0.33f, 0.5f)),
        swordRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {}

    /**
     * @brief Set the position of the player.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param z Z coordinate.
     */
    void SetPosition(float x, float y, float z) {
        Position = glm::vec3(x, y, z);
    }

    /**
     * @brief Get the position of the player.
     * @return Player's position as glm::vec3.
     */
    glm::vec3 GetPosition() const {
        return Position;
    }

    /**
     * @brief Get the damage dealt by the player.
     * @return Player's damage.
     */
    float GetDamage() {
        return Damage;
    }

    /**
     * @brief Increase the player's damage.
     * @param amount Amount to increase.
     */
    void addDamage(float amount) {
        Damage += amount;
    }

    /**
     * @brief Increase the player's attack speed.
     * @param amount Amount to increase.
     */
    void addAttackSpeed(float amount) {
        attackDuration -= amount;
        returnDuration -= amount / 2;
        if (attackDuration < 0.1f) {
            attackDuration = 0.1f;
        }
        if (returnDuration < 0.1f) {
            returnDuration = 0.2f;
        }
    }

    /**
     * @brief Get the player's attack speed.
     * @return Player's attack duration.
     */
    float GetAttackSpeed() {
        return attackDuration;
    }

    /**
     * @brief Set the current health of the player.
     * @param health Health to set.
     */
    void setCurrentHealth(int health) {
        Health = health;
    }

    /**
     * @brief Get the player's current health.
     * @return Player's health.
     */
    float GetHealth() const {
        return Health;
    }

    /**
     * @brief Modify the player's current health.
     * @param amount Amount to modify.
     */
    void modifyCurrentHealth(int amount) {
        playDamageSound = false;
        Health += amount;
        if (Health < 0) Health = 0;
        if (Health > MaxHealth) Health = MaxHealth;
        setCurrentHealth(Health);
        wasAttacked = true;
        if (amount < 0 && !playDamageSound) {
            playPlayerTakeDamageSound();
            playDamageSound = true;
        }
    }

    /**
     * @brief Increase the player's maximum health.
     * @param amount Amount to increase.
     */
    void addMaxHealth(float amount) {
        MaxHealth += amount;
    }

    /**
     * @brief Get the player's maximum health.
     * @return Player's maximum health.
     */
    float GetMaxHealth() {
        return MaxHealth;
    }

    /**
     * @brief Get the player's attack range.
     * @return Player's attack range.
     */
    float GetAttackRange() {
        return Range;
    }

    /**
     * @brief Increase the player's attack range.
     * @param amount Amount to increase.
     */
    void addAttackRange(float amount) {
        Range += amount;
    }

    /**
     * @brief Get the player's health regeneration rate.
     * @return Player's health regeneration rate.
     */
    float GetRegeneration() {
        return Regeneration;
    }

    /**
     * @brief Increase the player's health regeneration rate.
     * @param amount Amount to increase.
     */
    void addRegeneration(float amount) {
        Regeneration += amount;
    }

    /**
     * @brief Print the player's position.
     */
    void printPlayerPosition() {
        //printf("Health: %d\n", Health);
        //printf("Position: (%f, %f)\n", Position.x, Position.z);
    }

    /**
     * @brief Calculate the distance from the player to a given point.
     * @param point The point to calculate distance to.
     * @return Distance to the point.
     */
    float CalculateDistance(glm::vec3 point) const {
        return glm::distance(Position, point);
    }

    /**
     * @brief Check the distance to a point and modify health based on the threshold.
     * @param point The point to check distance to.
     * @param threshold Distance threshold.
     * @param healthReduction Amount of health to reduce.
     * @param canDamage Flag to allow damage.
     * @param enemy Reference to the enemy.
     */
    void CheckDistanceAndModifyHealth(glm::vec3 point, float threshold, int healthReduction, bool canDamage, Enemy& enemy) {
        if (canDamage) {
            float distance = CalculateDistance(point);
            if (distance < threshold) {
                enemy.inRange = true;
                if (!enemy.isAnimating && !enemy.inAttackMode) {
                    enemy.isAnimating = true;
                    enemy.inAttackMode = true; // Start attack mode
                }
                if (enemy.damagePlayer) {
                    modifyCurrentHealth(-enemy.Damage);
                    enemy.damagePlayer = false;
                }
            }
            else {
                enemy.inRange = false;
                if (!enemy.isAnimating && enemy.animationFinished) {
                    enemy.inAttackMode = false; // End attack mode when animation is finished
                }
            }
        }
    }

    /**
     * @brief Regenerate health every second and check for movement.
     */
    void RegenerateHealthAndCheckForMovement() {
        glm::vec3 lastPosition = Position;
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = now - lastHealthUpdateTime;

        if (elapsedTime.count() >= 1.0f) {
            regenTime++;
            if (Position != lastPosition) {
                // Code to handle position change
            }
            lastHealthUpdateTime = now; // Reset the timer
            std::cout << "RegenTime: " << regenTime << std::endl;
            if (regenTime == 10) {
                modifyCurrentHealth(Regeneration);
                regenTime = 0;
            }
        }
    }

    /**
     * @brief Update the sword animation.
     * @param deltaTime Time elapsed since the last frame.
     * @param enemies List of enemies.
     */
    void UpdateSwordAnimation(float deltaTime, std::vector<Enemy>& enemies) {
        if (isAnimating) {
            animationTime += deltaTime;
            float t;

            if (!isReturning) {
                t = animationTime / attackDuration;
                if (!playAttack) {
                    playAttack = true;
                    playAtackSound();
                }
                // Interpolate sword position from right to left
                float startX = 0.23f; // Starting X offset
                float endX = -0.23f; // Ending X offset
                float currentX = glm::mix(startX, endX, t);

                float startZ = 0.5f; // Starting Z offset
                float endZ = 0.8f; // Ending Z offset
                float currentZ = glm::mix(startZ, endZ, t);

                swordOffset.x = currentX;
                swordOffset.z = currentZ;

                // Check if the sword has reached the end position
                if (animationTime >= attackDuration) {
                    playAttack = false;
                    // Check collisions with enemies
                    for (Enemy& enemy : enemies) {
                        glm::vec3 enemyPosition = enemy.GetPosition();
                        glm::vec2 playerPosition(Position.x, Position.z);
                        glm::vec2 enemyPos2D(enemyPosition.x, enemyPosition.z);
                        float distance = glm::distance(playerPosition, enemyPos2D);
                        if (distance < Range) {
                            enemy.modifyHealth(-Damage);
                            std::cout << "Enemy health: " << enemy.GetHealth() << std::endl << enemy.Position.z << std::endl;
                        }
                    }

                    animationTime = 0.0f; // Reset animation time
                    isReturning = true; // Start return phase
                }
            }
            else {
                t = animationTime / returnDuration;
                if (!playReturn) {
                    playReturn = true;
                    playAttackReturnSound();
                }
                // Interpolate sword position from left to right
                float startX = -0.23f; // Starting X offset
                float endX = 0.23f; // Ending X offset
                float currentX = glm::mix(startX, endX, t);

                float startZ = 0.8f; // Starting Z offset
                float endZ = 0.5f; // Ending Z offset
                float currentZ = glm::mix(startZ, endZ, t);

                swordOffset.x = currentX;
                swordOffset.z = currentZ;

                // Check if the sword has returned to the initial position
                if (animationTime >= returnDuration) {
                    playReturn = false;
                    isAnimating = false; // End animation
                    isReturning = false; // Reset return phase
                    swordOffset.x = 0.23f; // Reset sword offset
                    swordOffset.z = 0.5f; // Reset sword offset
                }
            }
        }
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> lastHealthUpdateTime; ///< Timer for health regeneration.
};

#endif // PLAYER_H