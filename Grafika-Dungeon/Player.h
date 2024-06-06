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
    int Damage = 20;        // Obra�enia zadawane przez gracza
    glm::vec3 swordOffset;
    glm::quat swordRotation; // Domy�lna rotacja miecza
    // Dodaj zmienne do �ledzenia stanu animacji w klasie Player lub Sword
    bool isAnimating = false;
    float animationTime = 0.0f;
    float attackDuration = 0.3f; // Czas trwania animacji ataku
    float returnDuration = 0.5f; // Czas trwania animacji powrotu
    bool isReturning = false;  // Nowa zmienna do �ledzenia fazy powrotu

    // Konstruktor
    Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f))
        : Position(position), lastHealthUpdateTime(std::chrono::high_resolution_clock::now()),
    	swordOffset(glm::vec3(0.3f, -0.3f, 0.8f)),
        swordRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {}

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

    // Metoda do obliczania odleg�o�ci od zadanego punktu
    float CalculateDistance(glm::vec3 point) const {
        return glm::distance(Position, point);
    }

    // Metoda do sprawdzania odleg�o�ci i modyfikacji zdrowia co sekund�
    void CheckDistanceAndModifyHealth(glm::vec3 point, float threshold, int healthReduction) {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = now - lastHealthUpdateTime;

        float distance = CalculateDistance(point);
        if (distance < threshold && elapsedTime.count() >= 1.0f) {
            ModifyHealth(-healthReduction);
            lastHealthUpdateTime = now; // reset the timer
        }
    }

    void UpdateSwordAnimation(float deltaTime) {
        if (isAnimating) {
            animationTime += deltaTime;
            float t;

            if (!isReturning) {
                t = animationTime / attackDuration;

                // Interpolacja pozycji miecza od prawej do lewej strony
                float startX = 0.3f;  // Pocz�tkowy X offset
                float endX = -0.3f;   // Ko�cowy X offset
                float currentX = glm::mix(startX, endX, t);

                // Aktualizacja pozycji miecza
                swordOffset.x = currentX;

                // Sprawd�, czy miecz osi�gn�� ko�cow� pozycj�
                if (animationTime >= attackDuration) {
                    animationTime = 0.0f;  // Resetuj czas animacji
                    isReturning = true;    // Rozpocznij faz� powrotu
                }
            }
            else {
                t = animationTime / returnDuration;

                // Interpolacja pozycji miecza od lewej do prawej strony
                float startX = -0.3f;  // Pocz�tkowy X offset
                float endX = 0.3f;     // Ko�cowy X offset
                float currentX = glm::mix(startX, endX, t);

                // Aktualizacja pozycji miecza
                swordOffset.x = currentX;

                // Sprawd�, czy miecz powr�ci� do pocz�tkowej pozycji
                if (animationTime >= returnDuration) {
                    isAnimating = false;  // Zako�cz animacj�
                    isReturning = false;  // Zresetuj faz� powrotu
                    swordOffset.x = 0.3f; // Przywr�� pocz�tkowy offset miecza
                }
            }
        }
    }


private:
    std::chrono::time_point<std::chrono::high_resolution_clock> lastHealthUpdateTime;
};

#endif // PLAYER_H
