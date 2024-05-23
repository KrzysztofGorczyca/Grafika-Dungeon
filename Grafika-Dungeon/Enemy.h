#ifndef ENEMY_H
#define ENEMY_H

#include "libs.h" // Include necessary libraries
#include "Camera.h" // Include the Camera class

class Enemy {
public:
    // Constructor
    Enemy(glm::vec3 position, float speed, float health)
        : Position(position), Speed(speed), Health(health) {}

    // Destructor
    ~Enemy() {}
    /*
    // Update function to handle movement and other logic
    void Update(float deltaTime, const Camera& camera) {
        // Example: Move towards the player
        glm::vec3 direction = glm::normalize(camera.getPosition() - Position);
        Position += direction * Speed * deltaTime;

        // Example: Check collision with the player
        if (glm::distance(Position, camera.getPosition()) < 1.0f) {
            // Handle collision with the player
           // player.TakeDamage(10); // For example, decrease player health
        }
    }
    */
    // Getters and setters
    glm::vec3 GetPosition() const { return Position; }
    float GetHealth() const { return Health; }
    
private:
    glm::vec3 Position;
    float Speed;
    float Health;
};

#endif // ENEMY_H