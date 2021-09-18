#version 330

in vec2 texCoord;

out vec4 fragColour;

uniform sampler2D sampler;
uniform float time;

void main() {
    fragColour = texture(sampler, vec2(texCoord.x + time, texCoord.y + time));
}
