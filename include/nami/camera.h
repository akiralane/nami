#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {

    const float SPEED = 0.005;

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

    Camera(glm::vec3 position, glm::vec3 worldUp);

    glm::mat4 getViewMatrix();
    void move(Direction direction);

private:

    void updateVectors();


};

#endif // CAMERA_H