#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <cmath>
#include <chrono>
#include "libs.h"
#include "Enemy.h"

class Player
{
public:
    glm::vec3 Position;     // Pozycja gracza na mapie
    float Health = 100.00;       // Zdrowie gracza
    float MaxHealth = 100.00;    // Maksymalne zdrowie gracza
    float Damage = 25;        // Obra¿enia zadawane przez gracza
    float Range = 1.50f;        // Zasiêg ataku gracza
    float Regeneration = 5.0f; // Regeneracja zdrowia gracza
    int regenTime = 0;
    glm::vec3 swordOffset;
    glm::quat swordRotation; // Domyœlna rotacja miecza
    // Dodaj zmienne do œledzenia stanu animacji w klasie Player lub Sword
    bool isAnimating = false;
    bool wasAttacked = false;
    float animationTime = 0.0f;
    float attackDuration = 0.4f; // Czas trwania animacji ataku
    float returnDuration = 0.8f; // Czas trwania animacji powrotu
    bool isReturning = false;  // Nowa zmienna do œledzenia fazy powrotu

    bool holdingE = false;  // Czy gracz trzyma klawisz E

    bool playAtack = false;
    bool playReturn = false;
    bool playDamageSound = false;

    // Konstruktor
    Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f))
        : Position(position), lastHealthUpdateTime(std::chrono::high_resolution_clock::now()),
        swordOffset(glm::vec3(0.23f, -0.33f, 0.5f)),
        swordRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {}

    // Metody do ustawiania i pobierania pozycji
    void SetPosition(float x, float y, float z) {
        Position = glm::vec3(x, y, z);
    }

    glm::vec3 GetPosition() const {
        return Position;
    }

    // Metody do modyfikacji obra¿eñ
    float GetDamage()
    {
        return Damage;
    }

    void addDamage(float amount)
    {
        Damage += amount;
    }

    //Metody do modyfikacji szybkoœci ataku
    void addAtackSpeed(float amount)
    {
        attackDuration -= amount;
        returnDuration -= amount / 2;
        if (attackDuration < 0.1f)
        {
            attackDuration = 0.1f;
        }
        if (returnDuration < 0.1f)
        {
            returnDuration = 0.2f;
        }
    }

    float GetAttackSpeed()
    {
        return attackDuration;
    }

    // Metody do modyfikacji zdrowia
    void setCurrentHealth(int health) {
        Health = health;
    }

    float GetHealth() const {
        return Health;
    }

    void modifyCurrentHealth(int amount) {
        playDamageSound = false;
        Health += amount;
        if (Health < 0) Health = 0;
        if (Health > MaxHealth) Health = MaxHealth;
        setCurrentHealth(Health);
        wasAttacked = true;
        if(amount<0 && !playDamageSound)
        {
            playPlayerTakeDamageSound();
            playDamageSound = true;
		}
    }

    void addMaxHealth(float amount)
    {
        MaxHealth += amount;
    }

    float GetMaxHealth()
    {
        return MaxHealth;
    }

    //Metody do modyfikacji zasiegu ataku
    float GetAttackRange()
    {
        return Range;
    }

    void addAttackRange(float amount)
    {
        Range += amount;
    }

    //Metody do modyfikacji regeneracji zdrowia
    float GetRegeneration()
    {
        return Regeneration;
    }

    void addRegeneration(float amount)
    {
        Regeneration += amount;
    }

    void printPlayerPosition() {
        //printf("Health: %d\n", Health);
        //printf("Position: (%f, %f)\n", Position.x, Position.z);
    }



    // Metoda do obliczania odleg³oœci od zadanego punktu
    float CalculateDistance(glm::vec3 point) const {
        return glm::distance(Position, point);
    }

    // Metoda do sprawdzania odleg³oœci i modyfikacji zdrowia co sekundê
    void CheckDistanceAndModifyHealth(glm::vec3 point, float threshold, int healthReduction, bool canDamage, Enemy& enemy) {
        if (canDamage) {
            float distance = CalculateDistance(point);
            if (distance < threshold) {
                enemy.inRange = true;
                if (!enemy.isAnimating && !enemy.inAttackMode) {
                    enemy.isAnimating = true;
                    enemy.inAttackMode = true; // Rozpocznij tryb ataku
                }
                if (enemy.damagePlayer) {
                    modifyCurrentHealth(-enemy.Damage);
                    enemy.damagePlayer = false;
                }
            }
            else {
                enemy.inRange = false;
                if (!enemy.isAnimating && enemy.animationFinished) {
                    enemy.inAttackMode = false; // Zakoñcz tryb ataku, gdy animacja jest zakoñczona
                }
            }
        }
    }


    // Metoda do regeneracji zdrowia co sekundê
    void RegenerateHealthAndCheckForMovement()
    {
        glm::vec3 lastPosition = Position;
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = now - lastHealthUpdateTime;
        
        if (elapsedTime.count() >= 1.0f)
        {
            regenTime++;
            if(Position!=lastPosition)
            {
	            
            }
            lastHealthUpdateTime = now; // reset the timer
            std::cout << "RegenTime: " << regenTime << std::endl;
            if (regenTime == 10)
            {
                modifyCurrentHealth(Regeneration);
                regenTime = 0;
            }
        }
    }

    // Metoda do aktualizacji animacji miecza
    void UpdateSwordAnimation(float deltaTime, std::vector<Enemy>& enemies) {
        if (isAnimating) {
            animationTime += deltaTime;
            float t;

            if (!isReturning) {
                t = animationTime / attackDuration;
                if(!playAtack)
                {
                    playAtack = true;
                	playAtackSound();
				}
                // Interpolacja pozycji miecza od prawej do lewej strony
                float startX = 0.23f;  // Pocz¹tkowy X offset
                float endX = -0.23f;   // Koñcowy X offset
                float currentX = glm::mix(startX, endX, t);

                // Interpolacja pozycji miecza od prawej do lewej strony
                float startZ = 0.5f;  // Pocz¹tkowy X offset
                float endZ = 0.8f;   // Koñcowy X offset
                float currentZ = glm::mix(startZ, endZ, t);

                // Aktualizacja pozycji miecza
                swordOffset.x = currentX;
                swordOffset.z = currentZ;


                // SprawdŸ, czy miecz osi¹gn¹³ koñcow¹ pozycjê
                if (animationTime >= attackDuration) {
                    playAtack = false;
                    // SprawdŸ kolizje z przeciwnikami
                    for (Enemy& enemy : enemies) {
                        glm::vec3 enemyPosition = enemy.GetPosition();
                        glm::vec2 playerPosition(Position.x, Position.z);
                        //printf("Player position -0.000978f, 15.233716f : (%f, %f)\n", Position.x, Position.z);
                        glm::vec2 enemyPos2D(enemyPosition.x, enemyPosition.z);
                        //printf("Enemy position -0.016375f, 10.050759f : (%f, %f)\n", enemyPosition.x, enemyPosition.z);
                        float distance = glm::distance(playerPosition, enemyPos2D);
                        //printf("Distance: %f\n", distance);
                        if (distance < Range) {
                            enemy.modifyHealth(-Damage);
                            std::cout << "Enemy health: " << enemy.GetHealth() << std::endl << enemy.Position.z << std::endl;
                        }
                    }

                    animationTime = 0.0f;  // Resetuj czas animacji
                    isReturning = true;    // Rozpocznij fazê powrotu

                }
            }
            else {
                t = animationTime / returnDuration;
                if (!playReturn)
                {
                    playReturn = true;
                    playAttackReturnSound();
                }
                // Interpolacja pozycji miecza od lewej do prawej strony
                float startX = -0.23f;  // Pocz¹tkowy X offset
                float endX = 0.23f;     // Koñcowy X offset
                float currentX = glm::mix(startX, endX, t);

                // Interpolacja pozycji miecza od prawej do lewej strony
                float startZ = 0.8f;  // Pocz¹tkowy X offset
                float endZ = 0.5f;   // Koñcowy X offset
                float currentZ = glm::mix(startZ, endZ, t);

                // Aktualizacja pozycji miecza
                swordOffset.x = currentX;
                swordOffset.z = currentZ;

                // SprawdŸ, czy miecz powróci³ do pocz¹tkowej pozycji
                if (animationTime >= returnDuration) {
                    playReturn = false;
                    isAnimating = false;  // Zakoñcz animacjê
                    isReturning = false;  // Zresetuj fazê powrotu
                    swordOffset.x = 0.23f; // Przywróæ pocz¹tkowy offset miecza
                    swordOffset.z = 0.5f; // Przywróæ pocz¹tkowy offset miecza
                }
            }
        }
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> lastHealthUpdateTime;
};

#endif // PLAYER_H
