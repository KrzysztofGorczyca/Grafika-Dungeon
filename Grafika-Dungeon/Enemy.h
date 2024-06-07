#ifndef ENEMY_H
#define ENEMY_H

#include "libs.h"
#include "Camera.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define ModelPath "Assets/Skeleton/skeleton.obj"

class enemyHand {
public:
    glm::vec3 position;
    glm::quat rotation;
    float rotationAngle = 0.0f;
    enemyHand(const glm::vec3& pos) : position(pos),
        rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
    {}

    void SetRotation(float angle) {
        rotation = glm::angleAxis(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
    }
};

class Enemy {
public:
    bool isAnimating = false;
    float animationTime = 0.0f;
    float attackDuration = 1.5f; // Czas trwania animacji ataku
    float returnDuration = 1.5f; // Czas trwania animacji powrotu
    bool isReturning = false;  // Nowa zmienna do œledzenia fazy powrotu
    bool onceDone = false;
    bool damagePlayer = false;
    bool animationFinished = false;
    bool inAttackMode = false;  // Dodana flaga
    glm::vec3 Position;
    glm::quat Rotation;
    float Speed;
    float Health = 100.f;
    float Damage = 10;
    float MaxHealth = 100.0f;
    enemyHand hand;
    bool attacked = false;

    // Constructor
    Enemy(glm::vec3 position)
        :Position(position)
        , Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        , Speed(1.0f)
        , hand(position + glm::vec3(0.088918f, 0.014702f, 0.77669f))
    {

    }
    bool canDamage = true;
    bool inRange = false;
    bool dealDamage = false;

    // Destructor
    ~Enemy() {}

    void playHandAnimation(float deltaTime) {
        if (inAttackMode) {
            animationTime += deltaTime;
            float t;

            if (!isReturning) {
                t = animationTime / attackDuration;
                // Interpolacja pozycji miecza od prawej do lewej strony
                float startX = 0.0f;  // Pocz¹tkowy X offset
                float endX = -70.0f;   // Koñcowy X offset
                float currentX = glm::mix(startX, endX, t);
                // Aktualizacja pozycji miecza
                hand.rotationAngle = currentX;
                // SprawdŸ, czy miecz osi¹gn¹³ koñcow¹ pozycjê
                if (animationTime >= attackDuration) {
                    damagePlayer = true;
                    animationTime = 0.0f;  // Resetuj czas animacji
                    isReturning = true;    // Rozpocznij fazê powrotu
                    attacked = true;
                }
            }
            else {
                t = animationTime / returnDuration;

                // Interpolacja pozycji miecza od lewej do prawej strony
                float startX = -70.0f;  // Pocz¹tkowy X offset
                float endX = 0.0f;     // Koñcowy X offset
                float currentX = glm::mix(startX, endX, t);

                hand.rotationAngle = currentX;

                // SprawdŸ, czy miecz powróci³ do pocz¹tkowej pozycji
                if (animationTime >= returnDuration) {
                    isAnimating = false;  // Zakoñcz animacjê
                    isReturning = false;  // Zresetuj fazê powrotu
                    animationTime = 0.0f;  // Resetuj czas animacji
                    damagePlayer = false;
                    hand.rotationAngle = 0.0f;
                    animationFinished = true; // Animacja zakoñczona
                    inAttackMode = false; // Atak zakoñczony
                }
            }
        }
    }

    // Update function to handle movement and other logic
    void Update(float deltaTime, const Camera& camera) {
        if (!isDead())
        {
            // Oblicz kierunek ruchu ignoruj¹c ró¿nicê w osi Y
            glm::vec3 direction = camera.getCameraPosition() - Position;
            direction.y = 0.0f;  // Ignoruj ró¿nicê w osi Y
            direction = glm::normalize(direction);  // Normalizuj wektor kierunku
            if (isDead())
            {
                canDamage = false;
                std::cout << "Enemy is dead" << std::endl;
            }
            
            // SprawdŸ odleg³oœæ i zaktualizuj pozycjê tylko w p³aszczyŸnie XZ
            float distance = glm::distance(glm::vec2(Position.x, Position.z), glm::vec2(camera.getCameraPosition().x, camera.getCameraPosition().z));
            if (distance < 5.0f && distance > 1.0f) {
                Position += direction * Speed * deltaTime;
                rotateEnemy(direction);
                if(direction.x<-69.0f)
                {
                    dealDamage = true;
                }
            }
            else if (distance < 1.0f) {
                rotateEnemy(direction);
            }

            // Ustaw wspó³rzêdn¹ Y na sta³¹ wartoœæ (np. 0)
            Position.y = 0.0f;
        }
    }

    // Metoda do modyfikacji zdrowia
    void modifyHealth(float amount)
    {
        Health += amount;
        if (Health < 0.0f) Health = 0.0f;
        if (Health > MaxHealth) Health = MaxHealth;
    }

    bool isDead()
    {
        if (Health <= 0.0f)
        {
            Position = glm::vec3(0.0f, 0.0f, 100.0f);
            return true;
        }
        return false;
    }

    // Getters and setters
    inline glm::vec3 GetPosition() const { return Position; }
    inline float GetHealth() const { return Health; }
    void UpdatePosition(glm::vec3 position)
    {
        Position = position;
    }


private:
    void rotateEnemy(glm::vec3 direction) {
        // Oblicz k¹t obrotu na podstawie wektora kierunku
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);  // Zak³adamy, ¿e pocz¹tkowy kierunek przeciwnika to oœ Z
        glm::vec2 dir2D = glm::normalize(glm::vec2(direction.x, direction.z));
        glm::vec2 forward2D = glm::normalize(glm::vec2(forward.x, forward.z));
        float angle = atan2(dir2D.y, dir2D.x) - atan2(forward2D.y, forward2D.x);

        // Utwórz kwaternion obrotu wokó³ osi Y
        glm::quat rotationQuat = glm::angleAxis(-angle, glm::vec3(0.0f, 1.0f, 0.0f));

        // Ustaw obrót przeciwnika
        Rotation = rotationQuat;
    }
};

#endif // ENEMY_H