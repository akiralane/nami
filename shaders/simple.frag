#version 330

in vec2 texCoord;

out vec4 fragColour;

uniform sampler2D sampler;

void main() {
    fragColour = texture(sampler, texCoord)
                 * vec4(0.8, 0.8, 1.0, 1.0); // blue tint for night sky
}
