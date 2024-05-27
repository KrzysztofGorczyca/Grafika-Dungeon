#ifndef ENEMY_H
#define ENEMY_H

#include "libs.h"
#include "Camera.h"


#define ModelPath "Assets/Enemy/Enemy.obj"

class Enemy {
public:
    glm::vec3 Position;
    float Speed;
    float Health;
    // Constructor
    Enemy(glm::vec3 position)
        :Position(position)
    {

    }

    // Destructor
    ~Enemy() {}

    // Update function to handle movement and other logic
    void Update(float deltaTime, const Camera& camera) {
        glm::vec3 direction = glm::normalize(camera.getCameraPosition() - Position);
        if (glm::distance(Position, camera.getCameraPosition()) > 1.0f)
            Position += direction * Speed * deltaTime;
    }

    // Getters and setters
    inline glm::vec3 GetPosition() const { return Position; }
    inline float GetHealth() const { return Health; }

private:
    
};

#endif // ENEMY_H