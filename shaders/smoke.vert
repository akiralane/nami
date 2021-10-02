#version 330

layout (location = 0) in vec3 pos;

out vec4 vertPos;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main() {

    vertPos = projectionMat * viewMat * modelMat * vec4(pos, 1.0);
    gl_Position = vertPos;

}
