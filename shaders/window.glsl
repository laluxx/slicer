#version 330 core

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0; // input texture
uniform vec2 resolution; // screen resolution
uniform vec2 rectSize; // size of the rectangle
uniform float cornerRadius; // radius of the rounded corners

void main() {
    vec2 p = (2.0 * fragTexCoord - resolution) / min(resolution.y, resolution.x);
    vec2 q = p - sign(p) * rectSize;
    float dist = length(max(q, 0.0)) - cornerRadius;

    if (dist < 0.0) {
        fragColor = texture(texture0, fragTexCoord);
    } else {
        discard;
    }
}
