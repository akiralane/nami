#include "nami/camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp) : front(glm::vec3(0, 0, -1)) {
    this->position = position;
    this->worldUp = worldUp;
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() {
    // (position of camera, position of what we are looking at, camera up vector)
    return glm::lookAt(position, position + front, up);
}

void Camera::move(Direction direction) {
    switch (direction) {
        case FORWARDS:
            position += front * SPEED;
            break;
        case BACKWARDS:
            position -= front * SPEED;
            break;
        case LEFT:
            position -= right * SPEED;
            break;
        case RIGHT:
            position += right * SPEED;
            break;
        case UP:
            position += worldUp * SPEED;
            break;
        case DOWN:
            position -= worldUp * SPEED;
            break;
    }
}

void Camera::updateVectors() {
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
