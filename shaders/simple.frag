#version 330

in vec2 texCoord;

out vec4 fragColour;

uniform sampler2D sampler; // declaring a sampler2D to assign the texture to in main

void main() {
    fragColour = texture(sampler, texCoord);
//    fragColour = vec4(1.0, 0.5, 0.2, 1.0);
}
