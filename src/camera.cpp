#include "nami/camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp) : front(glm::vec3(0, 0, -1)) {
    this->position = position;
    this->worldUp = worldUp;
    yaw = 0;
    pitch = 0;
    updateVectors();
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
            position += right *  SPEED;
            break;
        case UP:
            position += worldUp * SPEED;
            break;
        case DOWN:
            position -= worldUp * SPEED;
            break;
    }
}

void Camera::look(float deltaX, float deltaY) {

    yaw += deltaX * SENSITIVITY;
    pitch += deltaY * SENSITIVITY;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(direction);
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));

}

glm::mat4 Camera::getViewMatrix() {
    // (position of camera, position of what we are looking at, camera up vector)
    return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getPosition() { return position; }

void Camera::updateVectors() {
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
