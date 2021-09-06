#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 texCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main() {
    // note that we replace the z component with the w component - this means that it will always have a z
    // of 1.0 (max depth).
    // this is because perspective division gets run after the vertex shader, dividing the xyz
    // by w - therefore, any part of xyz which is equal to w will always be 1.0.
    gl_Position = (projectionMat * viewMat * modelMat * vec4(pos, 1.0)).xyww;
    texCoord = tex;
}
