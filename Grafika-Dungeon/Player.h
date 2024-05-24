#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <cmath>
class Player
{
public:
    glm::vec3 Position;     // Pozycja gracza na mapie
    int Health = 100;             // Zdrowie gracza
    int Damage = 20;             // Obra¿enia zadawane przez gracza

    // Konstruktor
    Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f))
        : Position(position) {}

    // Metody do ustawiania i pobierania pozycji
    void SetPosition(float x, float y, float z) {
        Position = glm::vec3(x, y, z);
    }

    glm::vec3 GetPosition() const {
        return Position;
    }

    void printPlayerPosition() {
        //printf("Health: %d\n", Health);
	}

    // Metody do modyfikacji zdrowia
    void SetHealth(int health) {
        Health = health;
    }

    int GetHealth() const {
        return Health;
    }

    void ModifyHealth(int amount) {
        Health += amount;
        if (Health < 0) Health = 0;
        if (Health > 100) Health = 100;
    }

    void ModifyStamina(int amount) {
        Health += amount;
        if (Health < 0) Health = 0;
        if (Health > 100) Health = 100;
    }

    // Metoda do obliczania odleg³oœci od zadanego punktu
    float CalculateDistance(glm::vec3 point) const {
        return glm::distance(Position, point);
    }

    // Metoda do sprawdzania odleg³oœci i modyfikacji zdrowia
    void CheckDistanceAndModifyHealth(glm::vec3 point, float threshold, int healthReduction) {
        float distance = CalculateDistance(point);
        if (distance < threshold) {
            ModifyHealth(-healthReduction);
        }
    }
};

#endif // PLAYER_H
