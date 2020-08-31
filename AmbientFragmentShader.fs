#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    vec3 ambientColor = vec3(1.0, 1.0, 1.0);
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * ambientColor;

    vec3 inputColor = texture(texture_diffuse1, TexCoords).xyz;
    vec3 result = ambient * inputColor;
    FragColor = vec4(result, 1.0);
}