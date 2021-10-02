#version 330

in vec4 vertPos;

out vec4 fragColour;

void main() {

    // fade alpha after a point
    float alpha = (vertPos.y > 0.2) ? (-0.1*vertPos.y + 1) : ( 0.8 );

    fragColour = vec4(1.0f, 1.0f, 1.0f, alpha);

}
