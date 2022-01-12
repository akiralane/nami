#version 330

in vec2 texCoord;

out vec4 fragColour;

uniform sampler2D sampler;
uniform float time; // used to scroll the texture over time

void main() {
    fragColour = texture(sampler, vec2(texCoord.x + time, texCoord.y + time))
                 * vec4(1.0, 0.8, 1.0, 1.0); // blue tint for night sky
}
