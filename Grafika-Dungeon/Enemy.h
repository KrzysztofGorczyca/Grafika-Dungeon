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
    bool isReturning = false;  // Nowa zmienna do �ledzenia fazy powrotu
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
                float startX = 0.0f;  // Pocz�tkowy X offset
                float endX = -70.0f;   // Ko�cowy X offset
                float currentX = glm::mix(startX, endX, t);
                // Aktualizacja pozycji miecza
                hand.rotationAngle = currentX;
                // Sprawd�, czy miecz osi�gn�� ko�cow� pozycj�
                if (animationTime >= attackDuration) {
                    damagePlayer = true;
                    animationTime = 0.0f;  // Resetuj czas animacji
                    isReturning = true;    // Rozpocznij faz� powrotu
                    attacked = true;
                }
            }
            else {
                t = animationTime / returnDuration;

                // Interpolacja pozycji miecza od lewej do prawej strony
                float startX = -70.0f;  // Pocz�tkowy X offset
                float endX = 0.0f;     // Ko�cowy X offset
                float currentX = glm::mix(startX, endX, t);

                hand.rotationAngle = currentX;

                // Sprawd�, czy miecz powr�ci� do pocz�tkowej pozycji
                if (animationTime >= returnDuration) {
                    isAnimating = false;  // Zako�cz animacj�
                    isReturning = false;  // Zresetuj faz� powrotu
                    animationTime = 0.0f;  // Resetuj czas animacji
                    damagePlayer = false;
                    hand.rotationAngle = 0.0f;
                    animationFinished = true; // Animacja zako�czona
                    inAttackMode = false; // Atak zako�czony
                }
            }
        }
    }

    // Update function to handle movement and other logic
    void Update(float deltaTime, const Camera& camera) {
        if (!isDead())
        {
            // Oblicz kierunek ruchu ignoruj�c r�nic� w osi Y
            glm::vec3 direction = camera.getCameraPosition() - Position;
            direction.y = 0.0f;  // Ignoruj r�nic� w osi Y
            direction = glm::normalize(direction);  // Normalizuj wektor kierunku
            if (isDead())
            {
                canDamage = false;
                std::cout << "Enemy is dead" << std::endl;
            }
            
            // Sprawd� odleg�o�� i zaktualizuj pozycj� tylko w p�aszczy�nie XZ
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

            // Ustaw wsp�rz�dn� Y na sta�� warto�� (np. 0)
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
        // Oblicz k�t obrotu na podstawie wektora kierunku
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);  // Zak�adamy, �e pocz�tkowy kierunek przeciwnika to o� Z
        glm::vec2 dir2D = glm::normalize(glm::vec2(direction.x, direction.z));
        glm::vec2 forward2D = glm::normalize(glm::vec2(forward.x, forward.z));
        float angle = atan2(dir2D.y, dir2D.x) - atan2(forward2D.y, forward2D.x);

        // Utw�rz kwaternion obrotu wok� osi Y
        glm::quat rotationQuat = glm::angleAxis(-angle, glm::vec3(0.0f, 1.0f, 0.0f));

        // Ustaw obr�t przeciwnika
        Rotation = rotationQuat;
    }
};

#endif // ENEMY_H