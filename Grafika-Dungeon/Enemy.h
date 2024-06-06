#ifndef ENEMY_H
#define ENEMY_H

#include "libs.h"
#include "Camera.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define ModelPath "Assets/Skeleton/skeleton.obj"

class Enemy {
public:
    glm::vec3 Position;
    glm::quat Rotation;
    float Speed;
    float Health;
    // Constructor
    Enemy(glm::vec3 position)
        :Position(position)
        , Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
        , Health(100.0f)
        , Speed(1.0f)
    {

    }

    // Destructor
    ~Enemy() {}

    // Update function to handle movement and other logic
    void Update(float deltaTime, const Camera& camera) {
        // Oblicz kierunek ruchu ignoruj�c r�nic� w osi Y
        glm::vec3 direction = camera.getCameraPosition() - Position;
        direction.y = 0.0f;  // Ignoruj r�nic� w osi Y
        direction = glm::normalize(direction);  // Normalizuj wektor kierunku

        // Sprawd� odleg�o�� i zaktualizuj pozycj� tylko w p�aszczy�nie XZ
        float distance = glm::distance(glm::vec2(Position.x, Position.z), glm::vec2(camera.getCameraPosition().x, camera.getCameraPosition().z));
        if (distance < 5.0f && distance > 1.0f) {
            Position += direction * Speed * deltaTime;
            rotateEnemy(direction);
        }else if(distance < 1.0f){
            rotateEnemy(direction);
        }

        // Ustaw wsp�rz�dn� Y na sta�� warto�� (np. 0)
        Position.y = 0.0f;
    }





    // Getters and setters
    inline glm::vec3 GetPosition() const { return Position; }
    inline float GetHealth() const { return Health; }

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