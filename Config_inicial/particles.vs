#version 330 core
layout (location = 0) in vec3 aPos; // Posición de la partícula

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float particleSize; // Para controlar el tamaño desde C++

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = particleSize; // Tamaño en píxeles del punto
}
