#version 330 core

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0; // input texture
uniform vec2 resolution; // screen resolution
uniform float blurRadius; // adjustable blur radius

const int samples = 9;
const float weights[samples] = float[](
    0.05, 0.09, 0.12, 0.15, 0.16, 0.15, 0.12, 0.09, 0.05
);

void main() {
    vec2 texOffset = 1.0 / resolution;
    vec4 result = texture(texture0, fragTexCoord) * weights[4];

    for (int i = 1; i < samples / 2 + 1; i++) {
        result += texture(texture0, fragTexCoord + vec2(texOffset.x * i, 0.0) * blurRadius) * weights[4 + i];
        result += texture(texture0, fragTexCoord - vec2(texOffset.x * i, 0.0) * blurRadius) * weights[4 - i];
        result += texture(texture0, fragTexCoord + vec2(0.0, texOffset.y * i) * blurRadius) * weights[4 + i];
        result += texture(texture0, fragTexCoord - vec2(0.0, texOffset.y * i) * blurRadius) * weights[4 - i];
    }

    fragColor = result;
}
