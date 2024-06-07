#pragma once
#include "libs.h"
#include "Camera.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Player.h"

/**
 * @brief Enumeration for player attributes that can be modified by a chest.
 */
enum class PlayerAttribute {
    MaxHealth,      ///< Maximum health of the player
    CurrentHealth,  ///< Current health of the player
    Damage,         ///< Damage dealt by the player
    AttackSpeed,    ///< Attack speed of the player
    Regeneration,   ///< Health regeneration rate of the player
    Range           ///< Attack range of the player
};

/**
 * @brief Class representing a chest in the game, which can provide bonuses to the player.
 */
class Chest {
public:
    glm::vec3 Position; ///< Position of the chest
    glm::quat Rotation; ///< Rotation of the chest
    float rotationRadians; ///< Rotation angle of the chest in radians
    PlayerAttribute attribute; ///< Attribute that the chest provides
    bool opened = false; ///< Indicates if the chest has been opened

    /**
     * @brief Constructs a Chest object.
     *
     * @param position Initial position of the chest.
     * @param rotation Initial rotation of the chest in radians.
     */
    Chest(glm::vec3 position, float rotation)
        : Position(position)
        , Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        , rotationRadians(rotation)
        , attribute(RandomAttribute())
    {}

    /**
     * @brief Gets the position of the chest.
     *
     * @return The current position of the chest.
     */
    inline glm::vec3 GetPosition() const { return Position; }

    /**
     * @brief Gets the rotation angle of the chest in radians.
     *
     * @return The current rotation angle in radians.
     */
    inline float getRadians() const { return rotationRadians; }

    /**
     * @brief Gets the attribute provided by the chest.
     *
     * @return The attribute provided by the chest.
     */
    inline PlayerAttribute getAttribute() const { return attribute; }

    /**
     * @brief Checks if the player is interacting with the chest and applies the attribute if so.
     *
     * @param player The player interacting with the chest.
     */
    void CheckInteraction(Player& player) {
        glm::vec3 playerPosition = player.GetPosition();
        float distance = glm::distance(playerPosition, Position);
        if (distance <= 2.0f && player.holdingE == true) {
            ApplyAttribute(player);
            playOpenChestSound();
            opened = true;
        }
    }

private:
    /**
     * @brief Randomly selects a player attribute.
     *
     * @return A randomly selected player attribute.
     */
    PlayerAttribute RandomAttribute() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> randomAttribute(0, 5);

        return static_cast<PlayerAttribute>(randomAttribute(gen));
    }

    /**
     * @brief Randomly selects a value for the attribute.
     *
     * @return A randomly selected value for the attribute.
     */
    float RandomValue() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> randomValue(0, 10);
        return randomValue(gen);
    }

    /**
     * @brief Applies the attribute to the player.
     *
     * @param player The player to apply the attribute to.
     */
    void ApplyAttribute(Player& player) {
        switch (attribute) {
        case PlayerAttribute::MaxHealth: {
            float before = player.GetMaxHealth();
            player.addMaxHealth(RandomValue());
            float after = player.GetMaxHealth();
            player.modifyCurrentHealth((after - before) / 2);
            break;
        }
        case PlayerAttribute::CurrentHealth:
            player.modifyCurrentHealth(RandomValue());
            break;
        case PlayerAttribute::Damage:
            player.addDamage(RandomValue());
            break;
        case PlayerAttribute::AttackSpeed:
            player.addAttackSpeed(RandomValue() / 150);
            break;
        case PlayerAttribute::Range:
            player.addAttackRange(RandomValue() / 8);
            break;
        case PlayerAttribute::Regeneration:
            player.addRegeneration(RandomValue());
            break;
        }
    }
};