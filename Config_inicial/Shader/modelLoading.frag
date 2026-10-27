#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    
    // Si la textura falla o es negra, ponemos un color base (gris claro) para que se vea el modelo
    if(texColor.a < 0.1 && texColor.rgb == vec3(0.0)) {
        FragColor = vec4(0.7, 0.7, 0.7, 1.0); 
    } else {
        FragColor = texColor;
    }
}