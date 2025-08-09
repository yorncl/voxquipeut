#version 460 core
out vec4 FragColor;

uniform float opacity;
uniform vec3 lightpos;

in vec3 color;
in vec3 normal;
in vec3 fragpos;

void main()
{
    // FragColor = vec4(color, opacity);

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightpos - fragpos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0) * 0.6;
    vec3 ambient = 0.1 * vec3(1.0, 1.0, 1.0);


    // FragColor = vec4((normal+vec3(1.0, 1.0, 1.0))/2, opacity);
    FragColor = vec4(min(ambient + diffuse, 1.0), opacity);
} 
