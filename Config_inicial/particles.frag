#version 330 core
out vec4 FragColor;

uniform vec3 color;
uniform float alpha; // Desvanecimiento (fade out)

void main() {
    // gl_PointCoord da coordenadas de 0 a 1 dentro del punto flotante
    // Esto hace que el punto sea una esfera difuminada en lugar de un cuadrado rígido
    float distance = length(gl_PointCoord - vec3(0.5).xy);
    if (distance > 0.5) {
        discard;
    }
    
    float intensity = 1.0 - (distance * 2.0);
    FragColor = vec4(color * intensity, alpha);
}
