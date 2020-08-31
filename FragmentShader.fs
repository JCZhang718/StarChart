#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
// The position, in world space
in vec3 WorldPos;
// Not normalized
in vec3 VertexNormal;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;

void main()
{
    // phong illumination model: ambient + diffuse + specular
    // ambient
    vec3 ambientColor = vec3(1.0, 1.0, 1.0);
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * ambientColor;

    // diffuse - light source is the Sun
    vec3 lightPos = vec3(0, 0, 0);
    vec3 lightColor = vec3(1.0, 1.0, 0.8);
    vec3 normal = normalize(VertexNormal);
    vec3 lightDir = normalize(lightPos - WorldPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 inputColor = texture(texture_diffuse1, TexCoords).xyz;
    vec3 result = (ambient + diffuse + specular) * inputColor;
    FragColor = vec4(result, 1.0);
}