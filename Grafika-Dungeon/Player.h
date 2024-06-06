#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <cmath>
#include <chrono>

class Player
{
public:
    glm::vec3 Position;     // Pozycja gracza na mapie
    int Health = 100;       // Zdrowie gracza
    int Damage = 20;        // Obra¿enia zadawane przez gracza

    // Konstruktor
    Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f))
        : Position(position), lastHealthUpdateTime(std::chrono::high_resolution_clock::now()) {}

    // Metody do ustawiania i pobierania pozycji
    void SetPosition(float x, float y, float z) {
        Position = glm::vec3(x, y, z);
    }

    glm::vec3 GetPosition() const {
        return Position;
    }

    void printPlayerPosition() {
       //printf("Health: %d\n", Health);
       //printf("Position: (%f, %f)\n", Position.x, Position.z);
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

    // Metoda do obliczania odleg³oœci od zadanego punktu
    float CalculateDistance(glm::vec3 point) const {
        return glm::distance(Position, point);
    }

    // Metoda do sprawdzania odleg³oœci i modyfikacji zdrowia co sekundê
    void CheckDistanceAndModifyHealth(glm::vec3 point, float threshold, int healthReduction) {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = now - lastHealthUpdateTime;

        float distance = CalculateDistance(point);
        if (distance < threshold && elapsedTime.count() >= 1.0f) {
            ModifyHealth(-healthReduction);
            lastHealthUpdateTime = now; // reset the timer
        }
    }




private:
    std::chrono::time_point<std::chrono::high_resolution_clock> lastHealthUpdateTime;
};


#endif // PLAYER_H
