#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

    const float SPEED = 0.005;
    const float SENSITIVITY = 0.1;

public:

    enum Direction {
        FORWARDS,
        BACKWARDS,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    glm::vec3 position; // the camera's position in world space
    glm::vec3 front;    // front points out the front of the camera
    glm::vec3 up;       // up points out the top of the camera
    glm::vec3 right;    // ... you get the idea
    glm::vec3 worldUp;  // worldUp keeps track of 'up' in the world
    float yaw;
    float pitch;

    Camera(glm::vec3 position, glm::vec3 worldUp);

    void move(Direction direction);
    void look(float deltaX, float deltaY);

    glm::mat4 getViewMatrix();
    glm::vec3 getPosition();
    glm::vec2 getRotation();

};

#endif // CAMERA_H