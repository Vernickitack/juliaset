#version 330 core

in vec2 fragCoord;
out vec4 fragColor;

uniform vec2 iResolution;
uniform mat4 iViewMatrix;
uniform int iIterations;
uniform float iThreshold;
uniform vec2 iJuliaConstant;

vec3 palette(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);
    return a + b * cos(6.28318 * (c * t + d));
}

void main()
{
    float nx = fragCoord.x;
    float ny = fragCoord.y;
    float ratio = iResolution.x / iResolution.y;
    if (ratio > 1.0) {
        nx *= ratio;
    } else {
        ny /= ratio;
    }

    vec2 npos = vec2(nx, ny);

    vec4 bias = iViewMatrix * vec4(npos, 0.0, 1.0);
    vec2 z = vec2(bias.x, bias.y);

    vec2 c = iJuliaConstant;

    int iter = 0;

    for (int i = 0; i < iIterations; i++) {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        if (dot(z, z) > iThreshold * iThreshold) {
            iter = i;
            break;
        }
    }

    if (iter == iIterations - 1) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        float smoothIter = float(iter) - log2(log2(dot(z, z))) + 4.0;
        float percentage = smoothIter / float(iIterations);
        fragColor = vec4(palette(percentage), 1.0);
    }
}