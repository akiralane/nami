#version 330

in vec2 texCoord;

out vec4 fragColour;

uniform sampler2D sampler;

void main() {
    fragColour = texture(sampler, texCoord);
}
