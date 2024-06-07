#pragma once
#include "libs.h"
#include "Camera.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Player.h"

enum class PlayerAttribute {
    MaxHealth,
    CurrentHealth,
    Damage,
    AttackSpeed,
    Regeneration,
    Range
};

class Chest {
public:
    glm::vec3 Position;
    glm::quat Rotation;
    float rotationRadians;
    PlayerAttribute attribute;
    bool opened = false;

    // Constructor
    Chest(glm::vec3 position, float rotation)
        :Position(position)
        , Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        , rotationRadians(rotation)
        , attribute(RandomAttribute())
    {

    }
    inline glm::vec3 GetPosition() const { return Position; }
    inline float getRadians() const { return rotationRadians; }
    inline PlayerAttribute getAttribute() const { return attribute; }

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
    // Metoda losuj¹ca atrybut
    PlayerAttribute RandomAttribute() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> randomAttribute(0, 5);

        return static_cast<PlayerAttribute>(randomAttribute(gen));
    }
    // Metoda losuj¹ca wartoœæ atrybutu
    float RandomValue() {
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_real_distribution<> randomValue(0, 10);
		return randomValue(gen);
	}
    // Metoda aplikuj¹ca atrybut
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
            player.addAtackSpeed(RandomValue()/150);
            break;
		case PlayerAttribute::Range:
            player.addAttackRange(RandomValue()/8);
            break;
		case PlayerAttribute::Regeneration:
			player.addRegeneration(RandomValue());
			break;
        }
    }
};